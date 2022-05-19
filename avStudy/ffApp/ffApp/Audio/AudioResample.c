//
//  AudioResample.c
//  ffApp
//
//  Created by 苏莫离 on 2018/3/24.
//

#include "AudioResample.h"
#include <unistd.h>
#include "libavutil/avutil.h"
#include "libavdevice/avdevice.h"     /// 设备
#include "libavcodec/avcodec.h"       /// 使用 AVPacket
#include "libswresample/swresample.h" /// 重采样
/// 对于 ffmpeg 而言，所有的设备、多媒体文件，都是一种格式，如 PCM、WAV、
/// 对于格式的解析，统一使用 avformat
#include "libavformat/avformat.h"



/// 音频重采样上下文
SwrContext* init_swr(void){
    SwrContext *swr_ctx = NULL;

    //channel, number/
    swr_ctx = swr_alloc_set_opts(NULL,                //ctx
                                 AV_CH_LAYOUT_STEREO, //输出声道布局：立体声
                                 AV_SAMPLE_FMT_S16,   //输出采样格式：16 位
                                 44100,               //输出的采样率
                                 AV_CH_LAYOUT_STEREO, //输入声道布局：立体声
                                 AV_SAMPLE_FMT_FLT,   //输入采样格式：32位
                                 44100,               //输入的采样率
                                 0, NULL);
    
    if(!swr_ctx){
        
    }
    if(swr_init(swr_ctx) < 0){ ///  初始化
        
    }
    return swr_ctx;
}

static int isResampleing = 0;
void start_audioResample(const char *filePath) {
    isResampleing = 0;

    avdevice_register_all();
    const AVInputFormat *iformat = av_find_input_format("avfoundation");
    AVFormatContext *fmt_ctx = NULL;
    const char *devicename = ":0";
    AVDictionary *options = NULL;
    int ret = avformat_open_input(&fmt_ctx, devicename, iformat, &options);
    if (ret != 0) {
        char errors[1024] = {0,};
        av_strerror(ret, errors, 1024);
        av_log(NULL, AV_LOG_INFO, "Failed to open audio device, [%d]:%s\n",ret, errors);
        return;
    }
    
    FILE *outfile = fopen(filePath, "wb+");
    if (outfile == NULL) {
        avformat_close_input(&fmt_ctx);
        av_log(NULL, AV_LOG_INFO, "Failed to open file : %s\n",filePath);
        return;
    }
    
    isResampleing = 1;
    AVPacket pkt;
    av_init_packet(&pkt);
    
    /// 重采样上下文
    SwrContext* swr_ctx = init_swr();
    
    uint8_t **src_data = NULL;
    int src_linesize = 0;
    /// 创建输入缓冲区
    av_samples_alloc_array_and_samples(&src_data,         /// 输出缓冲区地址
                                       &src_linesize,     /// 缓冲区大小
                                       2,                 /// 声道数
                                       512,               /// 单声道采样个数：4096 / 4 / 2 = 512
                                       AV_SAMPLE_FMT_FLT, /// 采样格式
                                       0);
    uint8_t **dst_data = NULL;
    int dst_linesize = 0;
    /// 创建输出缓冲区
    av_samples_alloc_array_and_samples(&dst_data,         /// 输出缓冲区地址
                                       &dst_linesize,     /// 缓冲区大小
                                       2,                 /// 声道数
                                       512,               /// 单声道采样个数：4096 / 4 / 2 = 512
                                       AV_SAMPLE_FMT_S16, /// 采样格式
                                       0);
    
    while ( ((ret = av_read_frame(fmt_ctx, &pkt)) == 0 || ret == -35) && isResampleing == 1) {
        if (ret == -35) {
            usleep(1);
            continue;
        }
        memcpy(src_data[0], pkt.data, pkt.size);
        /// 写入数据之前，先进行重采样
        swr_convert(swr_ctx,  /// 重采样上下文
                    dst_data, /// 输出结果缓冲区
                    512,      /// 每个声道的采样数
                    (const uint8_t **)src_data, /// 输入缓冲区
                    512);     /// 输入单个声道的采样数
        
        fwrite(dst_data[0], dst_linesize, 1, outfile);
        av_log(NULL, AV_LOG_INFO, "size = %d\n", pkt.size);
    }
    
    /// 内存释放
    if (src_data != NULL) {
        av_free(&src_data[0]);
    }
    av_free(&src_data);
    if (dst_data != NULL) {
        av_free(&dst_data[0]);
    }
    av_free(&dst_data);
    swr_free(&swr_ctx);
    av_packet_unref(&pkt);
    avformat_close_input(&fmt_ctx);
    fclose(outfile);
}

void stop_audioResample(void) {
    isResampleing = 0;
}
