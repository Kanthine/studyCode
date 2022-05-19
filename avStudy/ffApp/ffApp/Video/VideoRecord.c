//
//  VideoRecord.c
//  ffApp
//
//  Created by 苏莫离 on 2018/3/24.
//

#include "VideoRecord.h"
#include <unistd.h>
#include "libavutil/avutil.h"
#include "libavdevice/avdevice.h"
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"

static int isRecording = 0;

void start_videoRecord(const char *filePath) {
    isRecording = 0;
    
    avdevice_register_all();/// 注册设备
    const AVInputFormat *iformat = av_find_input_format("avfoundation"); /// 设置采集方式
    AVFormatContext *fmt_ctx = NULL;
    AVDictionary *options = NULL;
    av_dict_set(&options, "video_size", "1280x720", 0);
    av_dict_set(&options, "framerate", "30", 0);
    av_dict_set(&options, "pixel_format", "nv12", 0);
    
    /// 将设备当作一个多媒体文件，也就是一个编码后的文件
    /// [[video device] : [audio device]]
    const char *devicename = "0"; /// 0:0 摄像头:麦克风， 1 桌面
    int ret = avformat_open_input(&fmt_ctx, devicename, iformat, &options); /// 打开音频设备
    
    if (ret != 0) { /// 打开设备失败
        char errors[1024] = {0,};
        av_strerror(ret, errors, 1024);
        av_log(NULL, AV_LOG_INFO, "Failed to open video device, [%d]:%s\n",ret, errors);
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
        /// 写文件: pkt.size 可能不是正确的数据大小
        /// uyvy422 size = 1280 * 720 * 2   = 1843200
        /// nv12    size = 1280 * 720 * 1.5 = 1382400
        fwrite(pkt.data, 1382400, 1, outFile);
        
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

void stop_videoRecord(void) {
    isRecording = 0;
}

/// [YUV 格式查询](https://www.fourcc.org/yuv.php)

/** 开始录制时，控制台打印的信息：
 [avfoundation @ 0x7f9fa1037000] Selected pixel format (yuv420p) is not supported by the input device.
 [avfoundation @ 0x7f9fa1037000] Supported pixel formats:
 [avfoundation @ 0x7f9fa1037000]   uyvy422
 [avfoundation @ 0x7f9fa1037000]   yuyv422
 [avfoundation @ 0x7f9fa1037000]   nv12
 [avfoundation @ 0x7f9fa1037000]   0rgb
 [avfoundation @ 0x7f9fa1037000]   bgr0
 [avfoundation @ 0x7f9fa1037000] Overriding selected pixel format to use uyvy422 instead.
 
 * 在终端播放
 * ffplay -video_size 1280x720 -pixel_format uyvy422 6.yuv
 */
 
