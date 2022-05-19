//
//  pushStream.c
//  ffPushStream
//
//  Created by i7y on 2022/4/13.
//

#include "pushStream.h"
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <stdlib.h>
#include <librtmp/rtmp.h>

/**
 * FLV Header 一共有 9 个字节
 *  1～3  signature: ‘F’ ‘L’ ‘V’
 *  4 : 1  版本号
 *  5 : 前5位保留是 0，第 6 位表示是否有 音频， 第 7 位保留，第 8 位是否有视频
 *  6～9 表示 header 大小
 */
static FILE* open_flv(char *flvPath) {
    
    FILE *fp = fopen(flvPath, "rb");/// 二进制读取
    if (!fp) {
        printf("Failed to open flv: %s\n",flvPath);
        return NULL;
    }
    
    fseek(fp, 9, SEEK_SET); /// 跳过 flv header
    fseek(fp, 4, SEEK_SET); /// 跳过 preTagSize
    return fp;
}


static RTMP* connect_rtmp_server(char *rtmpaddr) {
    /// 1、创建对象、并初始化
    RTMP *rt = RTMP_Alloc();
    if (!rt) {
        printf("No memory,, failed to alloc RTMP object!\n");
        goto __ERROR;
    }
    RTMP_Init(rt);
    
    /// 2、配置参数
    rt -> Link.timeout = 10; /// 超时时间
    RTMP_SetupURL(rt, rtmpaddr);
    RTMP_EnableWrite(rt);/// 显式设置推流；否则默认拉流

    /// 3、建立连接
    RTMP_Connect(rt, NULL);
    
    /// 4、创建流
    RTMP_ConnectStream(rt,     0);
    
    return rt;
    
__ERROR:
    
    if (rt) {
        RTMP_Close(rt);
        RTMP_Free(rt);
    }
    return NULL;
}

static RTMPPacket* alloc_packet(void) {
    RTMPPacket *packet = (RTMPPacket *)malloc(sizeof(RTMPPacket));
    if (!packet) {
        printf("No memory,, failed to alloc RTMPPacket!\n");
        goto __ERROR;
    }
    
    RTMPPacket_Alloc(packet, 64 * 1024); /// 64K
    RTMPPacket_Reset(packet);
    packet -> m_hasAbsTimestamp = 0;
    packet -> m_nChannel = 0x4; ///标识通道的通道号,前三个号已经被占了，因此会从第四个开始!
    return packet;
    
__ERROR:
    
    if (packet) {
        free(packet);
    }
    return NULL;
}

static int read_u8(FILE *file, unsigned int *u8) {
    unsigned int tmp;
    if(fread(&tmp, 1, 1, file) != 1){
        printf("Failed to read_u8!\n");
    }
    *u8 = tmp & 0xFF;
    return 0;
}

static int read_u24(FILE *file, unsigned int *u24) {
    unsigned int tmp;
    if(fread(&tmp, 1, 3, file) != 3){
        printf("Failed to read_u24!\n");
        return -1;
    }
    *u24 = ((tmp >> 16) & 0xFF) | ((tmp << 16) & 0xFF0000) | (tmp & 0xFF00);
    return 0;
}

static int read_u32(FILE *file, unsigned int *u32) {
    unsigned int tmp;
    if(fread(&tmp, 1, 4, file) != 4){
        printf("Failed to read_u32!\n");
        return -1;
    }
    *u32 = ((tmp >> 24) & 0xFF) | ((tmp >> 8) & 0xFF00) | \
           ((tmp << 8) & 0xFF0000) | ((tmp << 24) & 0xFF000000);
    
    return 0;
}

static int read_ts(FILE *file, unsigned int *time){
    unsigned int tmp;
    if(fread(&tmp, 1, 4, file) !=4) {
        printf("Failed to read_ts!\n");
        return -1;
    }
    *time = ((tmp >> 16) & 0xFF) | ((tmp << 16) & 0xFF0000) | (tmp & 0xFF00) | (tmp & 0xFF000000);
    return 0;
}


/**
 * FLV tag header
 * 第一个字节 tag type : 0x8 音频， 0x9 视频，0x12 script
 * 2～4 字节 tag body 长度: PretTagSize - Tag Header
 * 5～7 字节 时间戳，单位是毫秒，script 的时间戳是 0
 * 第八个字节 扩展时间戳: 5～8 四个字节
 * 9～11字节 streamID 由于 FLV 是文件 streamID无意义，因此该值为 0
 */
static int read_data(FILE *file, RTMPPacket **packet) {
    
    unsigned int tag_type;
    unsigned int tag_body_size;
    unsigned int time;
    unsigned int streamID;
    unsigned int tag_pre_size;
    
    if(read_u8(file, &tag_type)){
        goto __ERROR;
    }
    if(read_u24(file, &tag_body_size)){
        goto __ERROR;
    }
    if(read_ts(file, &time)){
        goto __ERROR;
    }
    if(read_u24(file, &streamID)){
        goto __ERROR;
    }
    printf("tag header, ts: %u, tt: %d, datasize:%d \n", time, tag_type, tag_body_size);
    
    int read_size = fread((*packet) -> m_body, 1, tag_body_size, file);
    if (read_size != tag_body_size) {
        printf("Failed to read tag body from flv, (datasize=%d, tds=%d)\n",read_size,tag_body_size);
        goto __ERROR;
    }
    
    /// 设置 packet 数据
    (*packet) -> m_headerType = RTMP_PACKET_SIZE_LARGE;
    (*packet) -> m_nTimeStamp = time;
    (*packet) -> m_packetType = tag_type;
    (*packet) -> m_nBodySize = tag_body_size;
    
    read_u32(file, &tag_pre_size);
    return 0;
__ERROR:
    return -1;
}

/// 向流媒体服务器推流
static void send_data(FILE *file, RTMP *rt) {
    /// 1、创建 RTMPPacket
    RTMPPacket *packet = alloc_packet();
    
    while (1) {
        /// 2、从 flv 文件读取数据
        
        /// 3、判断 RTMP 连接是否正常
        if (!RTMP_IsConnected(rt)) {
            printf("Disconne... \n");
            break;
        }
        
        /// 4、发送数据
        RTMP_SendPacket(rt, packet, 0);
    }
}

void push_stream(const char *flvPath, const char *targetURL) {
    /// 1、读取 flv 文件
    FILE *flvFile = open_flv("");
    
    /// 2、连接 RTMP 服务器
    RTMP *rtmp = connect_rtmp_server("");
    
    /// 3、推流 音视频数据
    send_data(flvFile, rtmp);
}
