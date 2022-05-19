#include "libavutil/log.h"
#include "libavformat/avio.h"
#include "libavformat/avformat.h"

#ifndef AV_WB32
#   define AV_WB32(p, val) do {                 \
        uint32_t d = (val);                     \
        ((uint8_t*)(p))[3] = (d);               \
        ((uint8_t*)(p))[2] = (d)>>8;            \
        ((uint8_t*)(p))[1] = (d)>>16;           \
        ((uint8_t*)(p))[0] = (d)>>24;           \
    } while(0)
#endif

#ifndef AV_RB16
#   define AV_RB16(x)                           \
    ((((const uint8_t*)(x))[0] << 8) |          \
      ((const uint8_t*)(x))[1])
#endif

/// 增加特征码 StartCode
static int alloc_and_copy(AVPacket *out,
                          const uint8_t *sps_pps, uint32_t sps_pps_size,
                          const uint8_t *in, uint32_t in_size) {
    uint32_t offset         = out->size;
    uint8_t nal_header_size = 4;
    int err;
    
    /// packet 扩容
    err = av_grow_packet(out, sps_pps_size + in_size + nal_header_size);
    if (err < 0) return err;
    
    if (sps_pps) /// 如果有 sps_pps 信息，则先拷贝 sps_pps
        memcpy(out->data + offset, sps_pps, sps_pps_size);
    
    memcpy(out->data + sps_pps_size + nal_header_size + offset, in, in_size);
    
    if (!offset) {
        AV_WB32(out->data + sps_pps_size, 1);
    } else {
        (out->data + offset + sps_pps_size)[0] =
        (out->data + offset + sps_pps_size)[1] = 0;
        (out->data + offset + sps_pps_size)[2] = 1;
    }
    return 0;
}

int h264_extradata_to_annexb(const uint8_t *codec_extradata, const int codec_extradata_size, AVPacket *out_extradata, int padding) {
    uint16_t unit_size  = 0;
    uint64_t total_size = 0;
    uint8_t *out        = NULL;
    uint8_t unit_nb     = 0;
    uint8_t sps_done    = 0;
    uint8_t sps_seen    = 0;
    uint8_t pps_seen    = 0;
    uint8_t sps_offset  = 0;
    uint8_t pps_offset  = 0;

    /**
     * AVCC
     * bits
     *  8   version ( always 0x01 )
     *  8   avc profile ( sps[0][1] )
     *  8   avc compatibility ( sps[0][2] )
     *  8   avc level ( sps[0][3] )
     *  6   reserved ( all bits on )
     *  2   NALULengthSizeMinusOne    // 这个值是（前缀长度-1），值如果是3，那前缀就是4，因为4-1=3
     *  3   reserved ( all bits on )
     *  5   number of SPS NALUs (usually 1)
     *
     *  repeated once per SPS:
     *  16     SPS size
     *
     *  variable   SPS NALU data
     *  8   number of PPS NALUs (usually 1)
     *  repeated once per PPS
     *  16    PPS size
     *  variable PPS NALU data
     */

    const uint8_t *extradata = codec_extradata + 4; //extradata存放数据的格式如上，前4个字节没用，所以将其舍弃
    static const uint8_t nalu_header[4] = { 0, 0, 0, 1 }; //每个H264裸数据都是以 0001 4个字节为开头的
    
    extradata++; //跳过一个字节，这个也没用
    
    sps_offset = pps_offset = -1;

    /* retrieve sps and pps unit(s) */
    unit_nb = *extradata++ & 0x1f; /* 取 SPS 个数，理论上可以有多个, 但我没有见到过多 SPS 的情况*/
    if (!unit_nb) {
        goto pps;
    }else {
        sps_offset = 0;
        sps_seen = 1;
    }

    while(unit_nb--) {
        int err;

        unit_size   = AV_RB16(extradata);
        total_size += unit_size + 4; //加上4字节的h264 header, 即 0001
        if (total_size > INT_MAX - padding) {
            av_log(NULL, AV_LOG_ERROR,
                   "Too big extradata size, corrupted stream or invalid MP4/AVCC bitstream\n");
            av_free(out);
            return AVERROR(EINVAL);
        }

        //2:表示上面 unit_size 的所占字结数
        //这句的意思是 extradata 所指的地址，加两个字节，再加 unit 的大小所指向的地址
        //是否超过了能访问的有效地址空间
        if (extradata + 2 + unit_size > codec_extradata + codec_extradata_size) {
            av_log(NULL, AV_LOG_ERROR, "Packet header is not contained in global extradata, "
                   "corrupted stream or invalid MP4/AVCC bitstream\n");
            av_free(out);
            return AVERROR(EINVAL);
        }

        //分配存放 SPS 的空间
        if ((err = av_reallocp(&out, total_size + padding)) < 0)
            return err;
        
        memcpy(out + total_size - unit_size - 4, nalu_header, 4);
        memcpy(out + total_size - unit_size, extradata + 2, unit_size);
        extradata += 2 + unit_size;
pps:
        //当 SPS 处理完后，开始处理 PPS
        if (!unit_nb && !sps_done++) {
            unit_nb = *extradata++; /* number of pps unit(s) */
            if (unit_nb) {
                pps_offset = total_size;
                pps_seen = 1;
            }
        }
    }

    //余下的空间清0
    if (out){
        memset(out + total_size, 0, padding);
    }

    if (!sps_seen)
        av_log(NULL, AV_LOG_WARNING, "Warning: SPS NALU missing or invalid. The resulting stream may not play.\n");

    if (!pps_seen)
        av_log(NULL, AV_LOG_WARNING, "Warning: PPS NALU missing or invalid. The resulting stream may not play.\n");

    out_extradata->data      = out;
    out_extradata->size      = total_size;
    
    return 0;
}

/// 解析 StartCode、SPS/PPS
int h264_mp4toannexb(AVFormatContext *fmt_ctx, AVPacket *in_pkt, FILE *dst_fd) {
    
    AVPacket *out_pkt = NULL;
    AVPacket spspps_pkt;
    
    size_t write_len;
    uint8_t unit_type;
    int32_t nal_size;
    uint32_t cumul_size    = 0;
    const uint8_t *buf;
    const uint8_t *buf_end;
    int            buf_size;
    int ret = 0, i;
    
    out_pkt = av_packet_alloc();
    
    buf      = in_pkt->data;
    buf_size = in_pkt->size;
    buf_end  = in_pkt->data + in_pkt->size; /// 结束地址：data地址后移size
    
    do {
        ret= AVERROR(EINVAL);
        //因为每个视频帧的前 4 个字节是视频帧的长度
        //如果buf中的数据都不能满足4字节，所以后面就没有必要再进行处理了
        if (buf + 4 > buf_end) goto fail;
        
        /** in_pkt->data 前4个字节，实际是 H264 帧的 size
         *  AVPacket  可能存储一个视频帧、也可能存储多个视频帧（主要根据帧的大小）
         *  从 AVPacket 拿取视频帧时，在每一帧开头获取 4个字节的视频帧大小
         *  for 循环主要做一个位移，取出前 4 个字节数据
         */
        /// 将前四字节转换成整型,也就是取出视频帧长度
        for (nal_size = 0, i = 0; i < 4; i++) nal_size = (nal_size << 8) | buf[i];
        
        /// 拿到视频帧大小，然后后跳 4 字节，从而指向真正的视频帧数据
        buf += 4;
        /// 视频帧的第一个字节的后5位是 NAL UNIT
        unit_type = *buf & 0x1f;
        
        /// 如果视频帧长度大于从 AVPacket 中读到的数据大小，说明数据包肯定出错了
        if (nal_size > buf_end - buf || nal_size < 0) goto fail;
        
        if (unit_type == 5) { /// 关键帧

            //在每个I帧之前都加 SPS/PPS
            h264_extradata_to_annexb(fmt_ctx->streams[in_pkt->stream_index]->codecpar->extradata,
                                     fmt_ctx->streams[in_pkt->stream_index]->codecpar->extradata_size,
                                     &spspps_pkt,
                                     AV_INPUT_BUFFER_PADDING_SIZE);

            if ((ret = alloc_and_copy(out_pkt,
                                      spspps_pkt.data, spspps_pkt.size,
                                      buf, nal_size)) < 0) goto fail;
                
        } else {
            if ((ret = alloc_and_copy(out_pkt, NULL, 0, buf, nal_size)) < 0) goto fail;
        }
        
        /// 将组织好的数据输出到目标文件中
        write_len = fwrite(out_pkt->data, 1, out_pkt->size, dst_fd);
        if(write_len != out_pkt->size){
            av_log(NULL, AV_LOG_DEBUG, "warning, length of writed data isn't equal pkt.size(%zu, %d)\n",
                   write_len, out_pkt->size);
        }
        fflush(dst_fd);

next_nal:
        buf        += nal_size;
        cumul_size += nal_size + 4; /// s->length_size;
    } while (cumul_size < buf_size);

fail:
    av_packet_free(&out_pkt);

    return ret;
}

int main(int argc, char *argv[]) {
    if(argc < 3){
        av_log(NULL, AV_LOG_DEBUG, "the count of parameters should be more than three!\n");
        return -1;
    }
    
    char *src_filename = argv[1];
    char *dst_filename = argv[2];
    if(src_filename == NULL || dst_filename == NULL){
        av_log(NULL, AV_LOG_ERROR, "src or dts file is null, plz check them!\n");
        return -1;
    }
    
    int err_code;
    char errors[1024];
    FILE *dst_fd = NULL;
    int video_stream_index = -1;
    AVFormatContext *fmt_ctx = NULL;
    AVPacket pkt;
    av_log_set_level(AV_LOG_DEBUG);
    /// 打开输出文件
    dst_fd = fopen(dst_filename, "wb");
    if (!dst_fd) {
        av_log(NULL, AV_LOG_DEBUG, "Could not open destination file %s\n", dst_filename);
        goto __ERROR;
    }
    
    /// 打开多媒体文件并创建多媒体上下文
     if((err_code = avformat_open_input(&fmt_ctx, src_filename, NULL, NULL)) < 0){
        av_strerror(err_code, errors, 1024);
        av_log(NULL, AV_LOG_DEBUG, "Could not open source file: %s, %d(%s)\n",
               src_filename,
               err_code,
               errors);
         goto __ERROR;
    }
    
    /// dump 输入流信息
    av_dump_format(fmt_ctx, 0, src_filename, 0);
    /// init packet
    av_new_packet(&pkt, 0);
    
    /// 查找视频流
    video_stream_index = av_find_best_stream(fmt_ctx, AVMEDIA_TYPE_VIDEO, -1, -1, NULL, 0);
    if(video_stream_index < 0){
        av_log(NULL, AV_LOG_DEBUG, "Could not find %s stream in input file %s\n",
               av_get_media_type_string(AVMEDIA_TYPE_VIDEO),src_filename);
        return AVERROR(EINVAL);
    }

    /// 判断是否是 H264 编码
    enum AVCodecID codec_id = fmt_ctx->streams[video_stream_index]->codecpar->codec_id;
    if(codec_id != AV_CODEC_ID_H264) {
        av_log(NULL, AV_LOG_ERROR, "the video type is not h264 !\n");
        goto __ERROR;
    }else{
        av_log(NULL, AV_LOG_INFO, "the video type is h264!\n");
    }
    
    /// 读取数据包
    while(av_read_frame(fmt_ctx, &pkt) >=0 ){
        if(pkt.stream_index == video_stream_index) {
            
            /// 将获取的视频包传递到下述函数
            /// 设置 StartCode、SPS/PPS
            h264_mp4toannexb(fmt_ctx, &pkt, dst_fd);
        }
        
        /// 解引用 pkt
        av_packet_unref(&pkt);
    }
    
__ERROR:
    if (fmt_ctx) avformat_close_input(&fmt_ctx);
    if (dst_fd) fclose(dst_fd);
    return 0;
}
