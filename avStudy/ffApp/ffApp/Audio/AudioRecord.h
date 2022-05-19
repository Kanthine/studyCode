//
//  AudioRecord.h
//  ffApp
//
//  Created by 苏莫离 on 2018/3/24.
//
// 音频录制: 本质上录制的是未编码 PCM 数据

#ifndef AudioRecord_h
#define AudioRecord_h

#include <stdio.h>

void start_audioRecord(const char *filePath);

void stop_audioRecord(void);

#endif


