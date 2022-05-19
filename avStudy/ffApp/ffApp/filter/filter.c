//
//  filter.c
//  ffApp
//
//  Created by 苏莫离 on 2018/3/24.
//

#include "filter.h"
#include "libavutil/avutil.h"
#include "libavformat/avformat.h"
#include "libavcodec/avcodec.h"
#include "libavfilter/avfilter.h"
#include "libavutil/opt.h"
#include "libavfilter/buffersrc.h"
#include "libavfilter/buffersink.h"


/**
 * @brief open media file
 * @param filename xxx
 * @param [out] fmt_ctx xxx
 * @param [out] dec_ctx xxx
 * @return 0: success, <0: failure
 */
static int open_input_file(const char *filename,
                           AVFormatContext **fmt_ctx,
                           AVCodecContext **dec_ctx,
                           int *v_stream_index){
    
    int ret = -1;
    AVCodec *dec = NULL;
    
    if(( ret = avformat_open_input(fmt_ctx, filename, NULL, NULL)) < 0){
        av_log(NULL, AV_LOG_ERROR, "Failed to open file %s\n", filename);
        return ret;
    }
    
    if((ret = avformat_find_stream_info((*fmt_ctx), NULL)) < 0){
        av_log(NULL, AV_LOG_ERROR, "Failed to find stream information!\n");
        return ret;
    }
    
    if ((ret = av_find_best_stream(*fmt_ctx, AVMEDIA_TYPE_VIDEO, -1, -1, &dec, 0)) < 0){
        av_log(NULL, AV_LOG_ERROR, "Can't find video stream!\n");
        return ret;
    }
    
    *v_stream_index = ret;
    
    *dec_ctx = avcodec_alloc_context3(dec);
    if(!(*dec_ctx)){
        return AVERROR(ENOMEM);
    }
    
    avcodec_parameters_to_context(*dec_ctx, (*fmt_ctx)->streams[*v_stream_index]->codecpar);
    
    if((ret = avcodec_open2(*dec_ctx, dec, NULL)) < 0){
        av_log(NULL, AV_LOG_ERROR, "Failed to open decoder!\n");
        return ret;
    }
    
    return 0;
}

/** filter 初始化
 *
 * 基本过程：
 * 1、创建 graph、avfilter_graph_alloc()，代表一个总的地图
 * 2、创建 buffer filter 和 buffersink filter
 * 3、分析 filter 描述符、并构建 AVFilterGraph
 * 4、使构建好的 AVFilterGraph 生效
 */
static int init_filters(const char *filter_desc,
                        AVFormatContext *fmt_ctx,
                        AVCodecContext *dec_ctx,
                        int v_stream_index,
                        AVFilterGraph **graph,
                        AVFilterContext **buf_ctx,
                        AVFilterContext **bufsink_ctx){
    
    int ret = -1;
    
    char args[512] = {};
    AVRational time_base = fmt_ctx->streams[v_stream_index]->time_base;
    
    AVFilterInOut *inputs = avfilter_inout_alloc(); /// filter 输入参数
    AVFilterInOut *outputs = avfilter_inout_alloc();/// filter 输出参数
    
    if(!inputs || !outputs){
        av_log(NULL, AV_LOG_ERROR, "No Memory when alloc inputs or outputs!\n");
        return AVERROR(ENOMEM);
    }
    
    *graph = avfilter_graph_alloc();
    if(!(*graph)) {
        av_log(NULL, AV_LOG_ERROR, "No Memory when create graph!\n");
        return AVERROR(ENOMEM);
    }
    
    const AVFilter *bufsrc = avfilter_get_by_name("buffer");
    if(!bufsrc){
        av_log(NULL, AV_LOG_ERROR, "Failed to get buffer filter!\n");
        return -1;
    }
    
    const AVFilter *bufsink = avfilter_get_by_name("buffersink");
    if(!bufsink){
        av_log(NULL, AV_LOG_ERROR, "Failed to get buffersink filter!\n");
        return -1;
    }
    
    //输入 buffer filter
    //"[in]drawbox=xxxx[out]"
    snprintf(args, 512,
             "video_size=%dx%d:pix_fmt=%d:time_base=%d/%d:pixel_aspect=%d/%d",
             dec_ctx->width, dec_ctx->height,
             dec_ctx->pix_fmt,
             time_base.num, time_base.den,
             dec_ctx->sample_aspect_ratio.num, dec_ctx->sample_aspect_ratio.den);
    
    if((ret = avfilter_graph_create_filter(buf_ctx, bufsrc, "in", args, NULL, *graph))<0){
        av_log(NULL, AV_LOG_ERROR, "Failed to create buffer filter context!\n");
        goto __ERROR;
    }
    
    //输出 buffer sink filter
    enum AVPixelFormat pix_fmts[] = {AV_PIX_FMT_YUV420P, AV_PIX_FMT_GRAY8, AV_PIX_FMT_NONE};
    if((ret =avfilter_graph_create_filter(bufsink_ctx, bufsink, "out", NULL, NULL, *graph)) < 0 ){
        av_log(NULL, AV_LOG_ERROR, "Failed to create buffer sink filter context!\n");
        goto __ERROR;
    }
    av_opt_set_int_list(*bufsink_ctx, "pix_fmts", pix_fmts, AV_PIX_FMT_NONE, AV_OPT_SEARCH_CHILDREN);

    //create in/out
    inputs->name = av_strdup("out"); /// 上一级的 out 数据
    inputs->filter_ctx = *bufsink_ctx; /// filter 上下文
    inputs->pad_idx = 0;            /// 数据端口
    inputs->next = NULL;            /// 下一个 filter
    
    outputs->name = av_strdup("in");
    outputs->filter_ctx = *buf_ctx;
    outputs->pad_idx = 0;
    outputs->next = NULL;
    
    //解析 filter 的描述字符串
    if((ret = avfilter_graph_parse_ptr(*graph, filter_desc, &inputs, &outputs, NULL)) < 0){
        av_log(NULL, AV_LOG_ERROR, "Failed to parse filter description!\n");
        goto __ERROR;
    }
    
    /// 使构建好的 AVFilterGraph 生效
    if((ret = avfilter_graph_config(*graph, NULL)) < 0){
        av_log(NULL, AV_LOG_ERROR, "Failed to config graph!\n");
    }

__ERROR:
    avfilter_inout_free(&inputs);
    avfilter_inout_free(&outputs);
    
    return ret;
}

///
static int do_frame(AVFrame *filt_frame, FILE *out){
    /// filt_frame->data[0] YUV 的 Y 数据
    ///
    fwrite(filt_frame->data[0], 1, filt_frame->width*filt_frame->height, out);
    fflush(out);
    return 0;
}

/** 滤镜处理
 * 1、将数据添加到 buffer filter 中
 * 2、从 buffer sink 中获取处理后的数据
 */
static int filter_video(AVFrame *frame,
                        AVFrame *filt_frame,
                        AVFilterContext *buf_ctx,
                        AVFilterContext *bufsink_ctx,
                        FILE *out){
    
    int ret;
    
    if ((ret = av_buffersrc_add_frame(buf_ctx, frame)) < 0 ){
        av_log(NULL, AV_LOG_ERROR, "Failed to feed to filter graph!\n");
        return ret;
    }
    
    while(1){
        ret = av_buffersink_get_frame(bufsink_ctx, filt_frame);
        if(ret == AVERROR(EAGAIN) || ret == AVERROR_EOF){
            break;
        }
        
        if(ret < 0) {
            return ret;
        }
        
        do_frame(filt_frame, out);
        av_frame_unref(filt_frame);
    }
    
    av_frame_unref(frame);
    
    return ret;
}


/** 解码视频帧并对视频帧进行滤镜处理
 * 1、从解码器读取已经解码的视频帧
 * 2、将解码后的视频帧输送到 filter
 */
static int decode_frame_and_filter(AVFrame *frame,
                                   AVFrame *filt_frame,
                                   AVCodecContext *dec_ctx,
                                   AVFilterContext *buf_ctx,
                                   AVFilterContext *bufsink_ctx,
                                   FILE *out){
    
    int ret = avcodec_receive_frame(dec_ctx, frame);
    if(ret < 0){
        /// EAGAIN 解码器缓冲区为空（没有向解码器传送数据）
        /// AVERROR_EOF 已经全部解码完毕
        if(ret != AVERROR_EOF && ret != AVERROR(EAGAIN)){
            av_log(NULL, AV_LOG_ERROR, "Error while receiving a frame from decoder!\n");
        }
        return ret;
    }
    
    return filter_video(frame,
                        filt_frame,
                        buf_ctx,
                        bufsink_ctx,
                        out);
}


/** 使用 filter 的步骤
 * 1、获得解码后的原始数据 PCM/YUV
 * 2、将数据添加到 buffer filter 中
 * 3、从 buffer sink 中获取处理好的数据
 * 4、当所有数据处理完成后，释放资源
 */
int filter_mian(const char* filename, const char* outfile) {
//    const char* filename = "/Users/lichao/Documents/new_killer.mp4";
//    const char* outfile = "/Users/lichao/Documents/new_killer.yuv";
    
    /***************************** 变量名定义 *******************************/
    int ret;
    FILE *out = NULL;
    
    AVFormatContext *fmt_ctx;
    AVCodecContext *dec_ctx;
    
    AVFilterGraph *graph = NULL;
    AVFilterContext *buf_ctx = NULL;
    AVFilterContext *bufsink_ctx = NULL;
    
    int v_stream_index = -1;

    AVPacket packet;
    AVFrame *frame = NULL;
    AVFrame *filt_frame = NULL;
    
    const char *filter_desc="drawbox=30:10:64:64:red";
    
    av_log_set_level(AV_LOG_DEBUG);
    
    /***************************** 变量初始化 *******************************/

    frame = av_frame_alloc();
    filt_frame = av_frame_alloc();
    if(!frame || !filt_frame){
        av_log(NULL, AV_LOG_ERROR, "No Memory to alloc frame\n");
        exit(-1);
    }
    
    out = fopen(outfile, "wb");
    if(!out){
        av_log(NULL, AV_LOG_ERROR, "Failed to open yuv file!\n");
        exit(-1);
    }
    
    if((ret = open_input_file(filename,
                              &fmt_ctx,
                              &dec_ctx,
                              &v_stream_index)) < 0) {
        av_log(NULL, AV_LOG_ERROR, "Failed to open media file\n");
        goto __ERROR;
    }else{
        if((ret = init_filters(filter_desc,
                               fmt_ctx,
                               dec_ctx,
                               v_stream_index,
                               &graph,
                               &buf_ctx,
                               &bufsink_ctx)) < 0){
            av_log(NULL, AV_LOG_ERROR, "Failed to initialize filter!\n");
            goto __ERROR;
        }
    }
    
    /// 从多媒体文件读取数据包
    while(1){
        
        if((ret = av_read_frame(fmt_ctx, &packet)) < 0){
            goto __ERROR;
        }
        
        if(packet.stream_index == v_stream_index){ /// 找到对应的视频流
            ret = avcodec_send_packet(dec_ctx, &packet); ///解码视频包
            if(ret < 0){
                av_log(NULL, AV_LOG_ERROR, "Failed to send avpakcet to decoder!\n");
                goto __ERROR;
            }
            
            if((ret = decode_frame_and_filter(frame,
                                              filt_frame,
                                              dec_ctx,
                                              buf_ctx,
                                              bufsink_ctx,
                                              out)) < 0){
                
                if(ret == AVERROR(EAGAIN) || ret == AVERROR_EOF){
                    continue;
                }
                
                av_log(NULL, AV_LOG_ERROR, "Failed to decode or filter!\n");
                goto __ERROR;
            }
        }
    }
    
__ERROR:
    if(graph){
        avfilter_graph_free(&graph);
    }
    
    if(dec_ctx){
        avcodec_free_context(&dec_ctx);
    }
    
    if(fmt_ctx){
        avformat_close_input(&fmt_ctx);
    }
    
    if(frame){
        av_frame_free(&frame);
    }
    
    if(filt_frame){
        av_frame_free(&filt_frame);
    }
    return ret;
}
