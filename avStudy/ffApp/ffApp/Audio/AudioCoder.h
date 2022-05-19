//
//  AudioCoder.h
//  ffApp
//
//  Created by 苏莫离 on 2018/3/24.
//
// 音频编解码

#ifndef AudioCoder_h
#define AudioCoder_h

#include <stdio.h>

void start_audioCoder(const char *filePath);

void stop_audioCoder(void);

#endif

/**
 * ffmpeg 编码过程：
 * 1、创建编码器
 * 2、创建上下文：做上一步与下一步的关联使用；
 * 3、打开编码器：处于工作模式
 * 4、传输数据给编码器：编码器内部有个缓冲区，一帧一帧的传入数据，之后开始编码
 *                 对于视频帧来说，可能很多视频帧才编码出一帧数据；所以不可能输入一帧，立刻输出一帧；
 * 5、编码：处于一个循环，一直在接收数据编码；
 * 6、释放资源
 */
