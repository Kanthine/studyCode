//
//  VideoCoder.h
//  ffApp
//
//  Created by 苏莫离 on 2018/3/24.
//
// 视频编码

#ifndef VideoCoder_h
#define VideoCoder_h

#include <stdio.h>

void start_videoCoder(const char *inputPath, const char *outPath);

void stop_videoCoder(void);

#endif


/** H264 编码基本步骤
 * 1、打开编码器: 在编码器中设置一些参数，如编码器id、GOP、码流大小、分辨率等；
 * 2、转换 NV12 到 YUV420P：如果格式不匹配、需要先进行一次转换，
 *      可以使用 ffmpeg 自身的 sw 来转换，
 *      也可以使用 libYUV 来实现转换；
 *      还可以按照数据格式，自行转换；
 * 3、准备编码数据 AVFrame
 * 4、H264 编码
 */
