//
//  Mp4_Convert_FLV.c
//  ffApp
//
//  Created by 苏莫离 on 2018/3/24.
//

#include "Mp4_Convert_FLV.h"
#include "libavutil/timestamp.h"
#include "libavformat/avformat.h"

static void log_packet(const AVFormatContext *fmt_ctx, const AVPacket *pkt, const char *tag) {
    AVRational *time_base = &fmt_ctx->streams[pkt->stream_index]->time_base;

    printf("%s: pts:%s pts_time:%s dts:%s dts_time:%s duration:%s duration_time:%s stream_index:%d\n",
           tag,
           av_ts2str(pkt->pts), av_ts2timestr(pkt->pts, time_base),
           av_ts2str(pkt->dts), av_ts2timestr(pkt->dts, time_base),
           av_ts2str(pkt->duration), av_ts2timestr(pkt->duration, time_base),
           pkt->stream_index);
}


int mp4_convert_flv(const char *mp4Url, const char *flvUrl) {
    const AVOutputFormat *ofmt = NULL;
    AVFormatContext *ifmt_ctx = NULL, *ofmt_ctx = NULL;
    AVPacket pkt;
    int ret, i;
    int stream_index = 0;
    int *stream_mapping = NULL;
    int stream_mapping_size = 0;
    
    /// 打开输入的多媒体文件，获取输入上下文
    if ((ret = avformat_open_input(&ifmt_ctx, mp4Url, 0, 0)) < 0) {
        fprintf(stderr, "Could not open input file '%s'", mp4Url);
        goto end;
    }
    /// 打印输入多媒体文件的信息
    av_dump_format(ifmt_ctx, 0, mp4Url, 0);

    
    
    /// 输出上下文
    avformat_alloc_output_context2(&ofmt_ctx, NULL, NULL, flvUrl);
    if (!ofmt_ctx) {
        fprintf(stderr, "Could not create output context\n");
        ret = AVERROR_UNKNOWN;
        goto end;
    }
    
    stream_mapping_size = ifmt_ctx->nb_streams;
    stream_mapping = av_calloc(stream_mapping_size, sizeof(*stream_mapping));
    if (!stream_mapping) {
        ret = AVERROR(ENOMEM);
        goto end;
    }

    ofmt = ofmt_ctx->oformat;

    /// 获取输入多媒体文件的各路流
    for (i = 0; i < ifmt_ctx->nb_streams; i++) {
        AVStream *out_stream;
        AVStream *in_stream = ifmt_ctx->streams[i];
        AVCodecParameters *in_codecpar = in_stream->codecpar;

        /// 过滤：仅保留视频流、音频流、字幕流；
        if (in_codecpar->codec_type != AVMEDIA_TYPE_AUDIO &&
            in_codecpar->codec_type != AVMEDIA_TYPE_VIDEO &&
            in_codecpar->codec_type != AVMEDIA_TYPE_SUBTITLE) {
            stream_mapping[i] = -1;
            continue;
        }

        stream_mapping[i] = stream_index++;

        /// 根据输入的每路流、创建对应的输出流
        out_stream = avformat_new_stream(ofmt_ctx, NULL);
        if (!out_stream) {
            fprintf(stderr, "Failed allocating output stream\n");
            ret = AVERROR_UNKNOWN;
            goto end;
        }

        /// 将输入流的编解码参数，拷贝到输出流；
        ret = avcodec_parameters_copy(out_stream->codecpar, in_codecpar);
        if (ret < 0) {
            fprintf(stderr, "Failed to copy codec parameters\n");
            goto end;
        }
        out_stream->codecpar->codec_tag = 0;
    }
    /// 打印输出多媒体文件的信息
    av_dump_format(ofmt_ctx, 0, flvUrl, 1);

    if (!(ofmt->flags & AVFMT_NOFILE)) {
        ret = avio_open(&ofmt_ctx->pb, flvUrl, AVIO_FLAG_WRITE);
        if (ret < 0) {
            fprintf(stderr, "Could not open output file '%s'", flvUrl);
            goto end;
        }
    }

    ret = avformat_write_header(ofmt_ctx, NULL);
    if (ret < 0) {
        fprintf(stderr, "Error occurred when opening output file\n");
        goto end;
    }

    while (1) {
        AVStream *in_stream, *out_stream;

        ret = av_read_frame(ifmt_ctx, &pkt);
        if (ret < 0) break;
        
        in_stream = ifmt_ctx->streams[pkt.stream_index];
        if (pkt.stream_index >= stream_mapping_size ||
            stream_mapping[pkt.stream_index] < 0) {
            av_packet_unref(&pkt);
            continue;
        }
        
        pkt.stream_index = stream_mapping[pkt.stream_index];
        out_stream = ofmt_ctx->streams[pkt.stream_index];
        log_packet(ifmt_ctx, &pkt, "in");
        
        /// 刻度转换：假如输入的音频流采样率是 44100HZ，输出的默认刻度为 1000，在不同的刻度之间如果不进行转换，音频播放就会出现问题
        pkt.pts = av_rescale_q_rnd(pkt.pts,
                                   in_stream->time_base,
                                   out_stream->time_base,
                                   AV_ROUND_NEAR_INF|AV_ROUND_PASS_MINMAX); /// pts 用于展示
        pkt.dts = av_rescale_q_rnd(pkt.dts,
                                   in_stream->time_base,
                                   out_stream->time_base,
                                   AV_ROUND_NEAR_INF|AV_ROUND_PASS_MINMAX); /// dts 用于解码
        pkt.duration = av_rescale_q(pkt.duration, in_stream->time_base, out_stream->time_base);
        pkt.pos = -1;
        log_packet(ofmt_ctx, &pkt, "out");
        
        ret = av_interleaved_write_frame(ofmt_ctx, &pkt);
        if (ret < 0) {
            fprintf(stderr, "Error muxing packet\n");
            break;
        }
        av_packet_unref(&pkt);
    }
    
    /// 写入尾部信息
    av_write_trailer(ofmt_ctx);
end:
    if (ifmt_ctx) avformat_close_input(&ifmt_ctx);
    if (ofmt_ctx && !(ofmt->flags & AVFMT_NOFILE)) avio_closep(&ofmt_ctx->pb);
    if (ofmt_ctx) avformat_free_context(ofmt_ctx);
    if (stream_mapping) av_freep(&stream_mapping);
    
    if (ret < 0 && ret != AVERROR_EOF) {
        fprintf(stderr, "Error occurred: %s\n", av_err2str(ret));
        return 1;
    }    
    return 0;
}
