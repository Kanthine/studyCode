//
//  AudioCoder.c
//  ffApp
//
//  Created by 苏莫离 on 2018/3/24.
//

#include "AudioCoder.h"
#include "libavutil/avutil.h"
#include "libavutil/time.h"
#include "libavdevice/avdevice.h"
#include "libavformat/avformat.h"
#include "libavcodec/avcodec.h"
#include "libswresample/swresample.h"
#include <string.h>


static int coder_status = 0;


#pragma mark - peivate method


/// 重采样
static SwrContext* init_swr(void){
    
    SwrContext *swr_ctx = NULL;
    
    //channel, number/
    swr_ctx = swr_alloc_set_opts(NULL,                //ctx
                                 AV_CH_LAYOUT_STEREO, //输出channel布局
                                 AV_SAMPLE_FMT_S16,   //输出的采样格式
                                 44100,               //采样率
                                 AV_CH_LAYOUT_STEREO, //输入channel布局
                                 AV_SAMPLE_FMT_FLT,   //输入的采样格式
                                 44100,               //输入的采样率
                                 0, NULL);
    
    if(!swr_ctx){
        
    }
    
    if(swr_init(swr_ctx) < 0){
        
    }
    
    return swr_ctx;
}

/// 打开输入设备并返回上下文
static AVFormatContext* open_device(){
    int ret = 0;
    char errors[1024] = {0, };
    
    //ctx
    AVFormatContext *fmt_ctx = NULL;
    AVDictionary *options = NULL;
    
    //[[video device]:[audio device]]
    char *devicename = ":0";
    //设置采集方式
    const AVInputFormat *iformat = av_find_input_format("avfoundation");
    //打开音频设备
    if((ret = avformat_open_input(&fmt_ctx, devicename, iformat, &options)) < 0 ){
        av_strerror(ret, errors, 1024);
        fprintf(stderr, "Failed to open audio device, [%d]%s\n", ret, errors);
        return NULL;
    }
    return fmt_ctx;
}

/// 创建并打开编码器，返回上下文
/// 需要重采样：libfdk_aac 要求采样大小为 16 位，需要重采样为 16 位再使用 libfdk_aac 编解码
static AVCodecContext* open_coder(void){
    
    /******* 1、创建编码器 ****/
    /// avcodec_find_encoder(AV_CODEC_ID_AAC);/// 通过 ID 查找编解码器  AV_CODEC_ID_OPUS
    const AVCodec *codec = avcodec_find_encoder_by_name("libfdk_aac"); /// 通过名字查找编解码器
      
    /******* 2、创建 codec 上下文 ****/
    AVCodecContext *codec_ctx = avcodec_alloc_context3(codec);
    codec_ctx->sample_fmt = AV_SAMPLE_FMT_S16;          //输入音频的采样大小： 匹配重采样的输出格式
    codec_ctx->channel_layout = AV_CH_LAYOUT_STEREO;    //输入音频的channel layout
    codec_ctx->channels = 2;                            //输入音频 channel 个数
    codec_ctx->sample_rate = 44100;                     //输入音频的采样率
    codec_ctx->bit_rate = 0; //AAC_LC: 128K, AAC HE: 64K, AAC HE V2: 32K
    /// 如果设置了 profile ，就不要设置 bit_rate ！只有当 bit_rate = 0 时，设置 profile 才起作用！
    codec_ctx->profile = FF_PROFILE_AAC_HE_V2; // ffmpeg 源码
    
    /******* 3、打开编码器 ****/
    if(avcodec_open2(codec_ctx, codec, NULL) < 0) return NULL;
    return codec_ctx;
}

/** 编码音频数据
 * AVFrame  存放未编码的数据
 * AVPacket 存放编码后的数据
 * 疑问？先前 av_read_frame() 读取的什么数据？
 *
 * 输入数据：一帧一帧的（音频帧或者视频帧），将数据送给编码器（AVCodecContext）
 *        编码器缓冲一部分数据，然后开始编码；
 * int avcodec_send_frame(AVCodecContext *avctx, const AVFrame *frame);
 *
 *  编码完成之后，用户需要通过下述函数获取数据；
 * int avcodec_receive_packet(AVCodecContext *avctx, AVPacket *avpkt);
 */
static void encode(AVCodecContext *ctx,FILE *output,
                   AVFrame *frame, AVPacket *pkt ){
    
    // 将数据送编码器
    int ret = avcodec_send_frame(ctx, frame);/// 一帧一帧的，将数据送给编码器
    
    // 如果 ret >= 0 说明数据设置成功
    while(ret >= 0){ /// 可能有一堆编码后的数据等待吐出来，此处需要 while 循环去取出待吐出的数据！
        
        // 获取编码后的音频数据,如果成功，需要重复获取，直到失败为止
        ret = avcodec_receive_packet(ctx, pkt); /// 编码后的数据，为 packet
        
        if(ret == AVERROR(EAGAIN) || ret == AVERROR_EOF) { /// 没有编码好的数据等待取出，或者数据量不够
            return;
        }else if( ret < 0){  /// 不可恢复的错误（编码器出问题）
            printf("Error, encoding audio frame\n");
            exit(-1);
        }
        
        // write file
        fwrite(pkt->data, 1, pkt->size, output);
        fflush(output);
    }
    return;
}

/// 输入 AVFrame
static AVFrame* create_frame(){
    
    /// AVFrame 仅仅是一层外壳，数据存放于 buf 中
    AVFrame *frame = av_frame_alloc(); /// 堆中分配内存
    
    //音频输入数据
    if(!frame){
        printf("Error, No Memory!\n");
        goto __ERROR;
    }
    
    // 设置关键参数 512 * 2（2个字节） * 2（2个声道） = 2048
    frame->nb_samples     = 512;                //单通道一个音频帧的采样数
    frame->format         = AV_SAMPLE_FMT_S16;  //每个采样的大小
    frame->channel_layout = AV_CH_LAYOUT_STEREO; //channel layout
    
    //alloc inner memory ： 编码前的数据有 2028 个字节
    av_frame_get_buffer(frame, 0); // 512 * 2（2个字节） * 2（2个声道） = 2048
    if(!frame->data[0]){
        printf("Error, Failed to alloc buf in frame!\n");
        //内存泄漏
        goto __ERROR;
    }
    
    return frame;
    
__ERROR:
    if(frame){
        av_frame_free(&frame);
    }
    return NULL;
}

/// 封装 buffer
static void alloc_data_4_resample(uint8_t ***src_data, int *src_linesize,
                                  uint8_t ***dst_data, int *dst_linesize){
    //4096/4 = 1024/2 = 512
    //创建输入缓冲区
    av_samples_alloc_array_and_samples(src_data,         //输出缓冲区地址
                                       src_linesize,     //缓冲区的大小
                                       2,                 //通道个数
                                       512,               //单通道采样个数
                                       AV_SAMPLE_FMT_FLT, //采样格式
                                       0);
    
    //创建输出缓冲区
    av_samples_alloc_array_and_samples(dst_data,         //输出缓冲区地址
                                       dst_linesize,     //缓冲区的大小
                                       2,                 //通道个数
                                       512,               //单通道采样个数
                                       AV_SAMPLE_FMT_S16, //采样格式
                                       0);
}

/**
 */
static void free_data_4_resample(uint8_t **src_data, uint8_t **dst_data){
    //释放输入输出缓冲区
    if(src_data){
        av_freep(&src_data[0]);
    }
    av_freep(src_data);
    
    if(dst_data){
        av_freep(&dst_data[0]);
    }
    av_freep(dst_data);
}

/**
 */
static void read_data_and_encode(AVFormatContext *fmt_ctx, //
                          AVCodecContext *c_ctx,
                          SwrContext* swr_ctx,
                          FILE *outfile){
    
    int ret = 0;
    int errcount = 0;
    char errors[1024] = {0, };
    
    //pakcet
    AVPacket pkt;
    AVFrame *frame = NULL;
    AVPacket *newpkt = NULL;
    
    //重采样缓冲区
    uint8_t **src_data = NULL;
    int src_linesize = 0;
    
    uint8_t **dst_data = NULL;
    int dst_linesize = 0;
    
    frame = create_frame();
    if(!frame){
        //printf(...)
        goto __ERROR;
    }
    
    newpkt = av_packet_alloc(); //分配编码后的数据空间
    if(!newpkt){
        printf("Error, Failed to alloc buf in frame!\n");
        goto __ERROR;
    }
    
    //分配重采样输入/输出缓冲区
    alloc_data_4_resample(&src_data, &src_linesize, &dst_data, &dst_linesize);
    
    //read data from device
    while(coder_status) {
        
        ret = av_read_frame(fmt_ctx, &pkt);
        if(ret < 0){
            av_strerror(ret, errors, 1024); //打印错误信息
            printf("err:%d, %s\n", ret, errors);
            
            if (ret == AVERROR(EAGAIN)) {
                //连续5次则退出
                if(5 == errcount++) break;
                av_usleep(100000); //如果设备没有准备好，那就等一小会
                continue;
            }
            break;
        }

        //清0
        errcount = 0;
        
        // 进行内存拷贝，按字节拷贝的
        memcpy((void*)src_data[0], (void*)pkt.data, pkt.size);
        
        //重采样
        swr_convert(swr_ctx,                    //重采样的上下文
                    dst_data,                   //输出结果缓冲区
                    512,                        //每个通道的采样数
                    (const uint8_t **)src_data, //输入缓冲区
                    512);                       //输入单个通道的采样数
        
        //将重采样的数据拷贝到 frame 中
        memcpy((void *)frame->data[0], dst_data[0], dst_linesize);
        
        //encode
        encode(c_ctx, outfile, frame, newpkt);
        
        av_packet_unref(&pkt); // release pkt
    }
    
    //强制将编码器缓冲区中的音频进行编码输出
    encode(c_ctx, outfile, NULL, newpkt);
    
__ERROR:
    //释放 AVFrame 和 AVPacket
    if(frame){
        av_frame_free(&frame);
    }
    
    if(newpkt){
        av_packet_free(&newpkt);
    }
    
    //释放重采样缓冲区
    free_data_4_resample(src_data, dst_data);
}


#pragma mark - public method

/// AVCodecContext 替代了 AVInputFormat
void start_audioCoder(const char *outPath) {
    //context
    AVFormatContext *fmt_ctx = NULL;
    AVCodecContext *c_ctx = NULL;
    SwrContext* swr_ctx = NULL;
    
    //set log level
    av_log_set_level(AV_LOG_DEBUG);
    
    //register audio device
    avdevice_register_all();
    
    //start record
    coder_status = 1;
    
    //create file
    FILE *outfile = fopen(outPath, "wb+");
    if(!outfile){
        printf("Error, Failed to open file!\n");
        goto __ERROR;
    }
    
    //打开设备
    fmt_ctx = open_device();
    if(!fmt_ctx){
        printf("Error, Failed to open device!\n");
        goto __ERROR;
    }
    
    //打开编码器上下文
    c_ctx = open_coder();
    if(!c_ctx){
        printf("...");
        goto __ERROR;
    }
    
    //初始化重采样上下文
    swr_ctx = init_swr();
    if(!swr_ctx){
        printf("Error, Failed to alloc buf in frame!\n");
        goto __ERROR;
    }
    
    //encode
    read_data_and_encode(fmt_ctx, c_ctx, swr_ctx, outfile);
    
__ERROR:
    //释放重采样的上下文
    if(swr_ctx){
        swr_free(&swr_ctx);
    }

    if(c_ctx){
        avcodec_free_context(&c_ctx);
    }
    
    //close device and release ctx
    if(fmt_ctx) {
        avformat_close_input(&fmt_ctx);
    }
    
    if(outfile){
        //close file
        fclose(outfile);
    }
    av_log(NULL, AV_LOG_DEBUG, "finish!\n");
    return;
}

void stop_audioCoder(void) {
    coder_status = 0;
    printf("结束录制");
}
