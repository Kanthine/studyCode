//
//  VideoCoder.c
//  ffApp
//
//  Created by 苏莫离 on 2018/3/24.
//

#include "VideoCoder.h"
#include <string.h>
#include <unistd.h>
#include "libavutil/avutil.h"
#include "libavdevice/avdevice.h"
#include "libavformat/avformat.h"
#include "libavcodec/avcodec.h"
#include "libswresample/swresample.h"


#define V_WIDTH 1280
#define V_HEIGTH 720

static int rec_status = 0;



//@brief
//return
static AVFormatContext* open_dev(){
    
    int ret = 0;
    char errors[1024] = {0, };
    
    //ctx
    AVFormatContext *fmt_ctx = NULL;
    AVDictionary *options = NULL;
    
    //[[video device]:[audio device]]
    //0: 机器的摄像头
    //1: 桌面
    char *devicename = "0";
    
    //register audio device
    avdevice_register_all();
    
    //get format
    const AVInputFormat *iformat = av_find_input_format("avfoundation");
    
    av_dict_set(&options, "video_size", "1280x720", 0);
    av_dict_set(&options, "framerate", "30", 0);
    av_dict_set(&options, "pixel_format", "nv12", 0);
    
    //open device
    if((ret = avformat_open_input(&fmt_ctx, devicename, iformat, &options)) < 0 ){
        av_strerror(ret, errors, 1024);
        av_log(NULL, AV_LOG_INFO, "Failed to open video device, [%d]:%s\n",ret, errors);
        return NULL;
    }
    
    return fmt_ctx;
}

static void open_encoder(int width, int height,
                         AVCodecContext **enc_ctx){
    
    int ret = 0;
    const AVCodec *codec = avcodec_find_encoder_by_name("libx264");///获取编码器
    if(!codec){
        av_log(NULL, AV_LOG_INFO, "Codec libx264 not found\n");
        exit(1);
    }
    
    *enc_ctx = avcodec_alloc_context3(codec); /// 获取上下文
    if(!enc_ctx){
        av_log(NULL, AV_LOG_INFO, "Could not allocate video codec context!\n");
        exit(1);
    }
    
    // SPS/PPS
    (*enc_ctx)->profile = FF_PROFILE_H264_HIGH_444; ///最高级别
    (*enc_ctx)->level = 50; //表示LEVEL是5.0
    
    //设置分辫率
    (*enc_ctx)->width = width;   //1280
    (*enc_ctx)->height = height; //720
    
    //GOP
    /// 如果 GOP 过小，则 I 帧会很多；I 帧太多，则码流会更大
    /// 如果 GOP 过大，则 I 帧会很少；一旦网络上 I 帧丢包，等到下一个 I 帧需要很长时间；
    (*enc_ctx)->gop_size = 25;
    (*enc_ctx)->keyint_min = 25; //option （最小可能 25 帧就会插入一个 I 帧）
    
    //设置B帧数据
    (*enc_ctx)->max_b_frames = 3; //option
    (*enc_ctx)->has_b_frames = 1; //option
    
    //参考帧的数量
    (*enc_ctx)->refs = 3;         //option
    
    // 设置待编码数据的像素格式
    (*enc_ctx)->pix_fmt = AV_PIX_FMT_YUV420P; /// 输入的YUV格式
    //设置码率（平均码率）
    (*enc_ctx)->bit_rate = 600000; //600kbps
    
    //设置帧率
    (*enc_ctx)->time_base = (AVRational){1, 25}; //帧与帧之间的间隔是time_base
    (*enc_ctx)->framerate = (AVRational){25, 1}; //帧率，每秒 25 帧
    
    ret = avcodec_open2((*enc_ctx), codec, NULL);
    if(ret < 0){
        av_log(NULL, AV_LOG_INFO, "Could not open codec: %s!\n", av_err2str(ret));
        exit(1);
    }
}

static AVFrame* create_frame(int width, int height){
    
    int ret = 0;
    AVFrame *frame = NULL;
    
    frame = av_frame_alloc();
    if(!frame){
        av_log(NULL, AV_LOG_INFO, "av_frame_alloc Error, No Memory!\n");
        goto __ERROR;
    }
    
    
    
    //设置参数
    frame->width = width;
    frame->height = height;
    frame->format = AV_PIX_FMT_YUV420P; ///数据格式
    
    //alloc inner memory
    ret = av_frame_get_buffer(frame, 32); //按 32 位对齐
    if(ret < 0){
        av_log(NULL, AV_LOG_INFO, "Error, Failed to alloc buffer for frame!\n");
        goto __ERROR;
    }
    return frame;
    
__ERROR:
    if(frame) av_frame_free(&frame);
    return NULL;
}

/// H264 编码
static void encodeData(AVCodecContext *enc_ctx,
                       AVFrame *frame, AVPacket *newpkt,
                       FILE *outfile){
    
    int ret = 0;
    if(frame){
        av_log(NULL, AV_LOG_INFO, "send frame to encoder, pts=%lld\n", frame->pts);
    }
    // 传送原始数据给编码器进行编码
    ret = avcodec_send_frame(enc_ctx, frame);
    if(ret < 0) {
        av_log(NULL, AV_LOG_INFO, "Error, Failed to send a frame for enconding!\n");
        exit(1);
    }
    
    // 从编码器获取编码好的数据
    while(ret >= 0) {
        ret = avcodec_receive_packet(enc_ctx, newpkt);
        
        // 如果编码器数据不足时会返回 EAGAIN, 或者到数据尾时会返回 AVERROR_EOF
        if(ret == AVERROR(EAGAIN) || ret == AVERROR_EOF){
            return;
        }else if (ret < 0){
            av_log(NULL, AV_LOG_INFO, "Error, Failed to encode!\n");
            exit(1);
        }
        
        fwrite(newpkt->data, 1, newpkt->size, outfile);
        av_packet_unref(newpkt); /// 每调用一次 receive_packet 会增加一次引用计数，需要减少一次
    }
}

void start_videoCoder(const char *inputPath, const char *outPath){
    
    int ret = 0;
    int base = 0;

    //pakcet
    AVPacket pkt;
    AVFormatContext *fmt_ctx = NULL;
    AVCodecContext *enc_ctx = NULL;
    
    //set log level
    av_log_set_level(AV_LOG_DEBUG);
    
    //start record
    rec_status = 1;
    
    //create file
    FILE *yuvoutfile = fopen(inputPath, "wb+");
    FILE *outfile = fopen(outPath, "wb+");
    
    //打开设备
    fmt_ctx = open_dev();
    
    //打开编码器
    open_encoder(V_WIDTH, V_HEIGTH, &enc_ctx);
    
    //创建 AVFrame
    AVFrame *frame = create_frame(V_WIDTH, V_HEIGTH);
    
    //创建编码后输出的Packet
    AVPacket *newpkt = av_packet_alloc();
    if(!newpkt){
        av_log(NULL, AV_LOG_INFO, "Error, Failed to alloc avpacket!\n");
        goto __ERROR;
    }

    //read data from device
    while(((ret = av_read_frame(fmt_ctx, &pkt)) == 0 || ret == -35) &&
          rec_status) {
        if (ret == -35) {
            usleep(1);
            continue;
        }
        int i =0;
        av_log(NULL, AV_LOG_INFO, "packet size is %d(%p)\n", pkt.size, pkt.data);
       
        /// YUV 数据分层存储
        //（宽 x 高）x (yuv420=1.5/yuv422=2/yuv444=3)
        //YYYYYYYYUVUV NV12
        //YYYYYYYYUUVV YUV420
        memcpy(frame->data[0], pkt.data, V_WIDTH * V_HEIGTH); //copy Y data
        //307200之后，是UV
        for(i = 0; i < V_WIDTH * V_HEIGTH / 4; i++){
            frame->data[1][i] = pkt.data[V_WIDTH * V_HEIGTH + i * 2]; /// copy U data
            frame->data[2][i] = pkt.data[V_WIDTH * V_HEIGTH + i * 2]; /// copy V data
        }
        
        fwrite(frame->data[0], 1, V_WIDTH * V_HEIGTH, yuvoutfile);     /// Y
        fwrite(frame->data[1], 1, V_WIDTH * V_HEIGTH / 4, yuvoutfile); /// U
        fwrite(frame->data[2], 1, V_WIDTH * V_HEIGTH / 4, yuvoutfile); /// V
        
        frame->pts = base++;
        encodeData(enc_ctx, frame, newpkt, outfile); /// h264 编码
        //
        av_packet_unref(&pkt); //release pkt
    }
    
    // 传送 NULL ，清空缓冲区数据
    encodeData(enc_ctx, NULL, newpkt, outfile);
    
__ERROR:
    if(yuvoutfile){
        //close file
        fclose(yuvoutfile);
    }
    
    //close device and release ctx
    if(fmt_ctx) {
        avformat_close_input(&fmt_ctx);
    }

    av_log(NULL, AV_LOG_DEBUG, "finish!\n");
    
    return;
}

void stop_videoCoder() {
    rec_status = 0;
}

