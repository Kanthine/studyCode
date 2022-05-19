//
//  AudioRecord.c
//  ffApp
//
//  Created by 苏莫离 on 2018/3/24.
//

#include "AudioRecord.h"
#include <unistd.h>
#include "libavutil/avutil.h"
#include "libavdevice/avdevice.h" /// 设备
#include "libavcodec/avcodec.h" /// 使用 AVPacket
/// 对于 ffmpeg 而言，所有的设备、多媒体文件，都是一种格式，如 PCM、WAV、
/// 对于格式的解析，统一使用 avformat
#include "libavformat/avformat.h"


/** ffmpeg 采集音频流程：
 *  1、打开输入设备：音频设备、视频设备
 *      1.1、注册设备
 *      1.2、设置采集方式
 *      1.3、打开音频设备
 *  2、数据包：获取数据；
 *  3、输出文件
 */

static int isRecording = 0;

void start_audioRecord(const char *filePath) {
    isRecording = 0;
    
    /********** 1、打开输入设备 **********/
    avdevice_register_all();/// 注册设备
    const AVInputFormat *iformat = av_find_input_format("avfoundation"); /// 设置采集方式
    AVFormatContext *fmt_ctx = NULL;
    AVDictionary *options = NULL;
    
    /// 将设备当作一个多媒体文件，也就是一个编码后的文件
    /// 实际上读取的是一个未编码 PCM 数据
    /// [[video device] : [audio device]]
    const char *devicename = ":0"; /// 给出一个设备名称
    int ret = avformat_open_input(&fmt_ctx, devicename, iformat, &options); /// 打开音频设备
    
    if (ret != 0) { /// 打开设备失败
        char errors[1024] = {0,};
        av_strerror(ret, errors, 1024);
        av_log(NULL, AV_LOG_INFO, "Failed to open audio device, [%d]:%s\n",ret, errors);
        return;
    }
    
    /// 创建文件: w 写数据 b 二进制文件 + 若文件不存在则创建
    FILE *outFile = fopen(filePath, "wb+");
    if (outFile == NULL) {
        av_log(NULL, AV_LOG_INFO, "Failed to open file: %s\n",filePath);
        return;
    }
    
    av_log(NULL, AV_LOG_INFO, "start Record\n");
    isRecording = 1;
    
    ///一般而言，AVPacket 都是存放编码后的数据，此处存放 pcm 数据
    AVPacket pkt;/// 在栈空间分配内存
    av_init_packet(&pkt);
    while (((ret = av_read_frame(fmt_ctx, &pkt)) == 0 || ret == -35) &&
           isRecording == 1) {
        if (ret == -35) {
            usleep(1);
            continue;
        }
        /// 写文件
        fwrite(pkt.data, pkt.size, 1, outFile);
        
        /// fwrite() 后文件可能没有立即写入磁盘，操作系统出于效率的考虑，可能会将文件写入系统缓冲区。
        /// 当数据量足够大时，才会将缓冲区的数据拷贝到磁盘中
        /// 当数据不能实时写入磁盘，当发生断电等突发状况，可能导致部分数据丢失
        /// fflush() 将数据立即写入磁盘，但会降低执行效率
        fflush(outFile);
        av_log(NULL, AV_LOG_INFO, "size = %d\n", pkt.size);
    }
    av_packet_unref(&pkt);
    avformat_close_input(&fmt_ctx);
    fclose(outFile);
    av_log(NULL, AV_LOG_INFO, "stop Record\n");    
}

void stop_audioRecord(void) {
    isRecording = 0;
}
