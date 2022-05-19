//
//  Test.c
//  ffApp
//
//  Created by i7y on 2022/4/22.
//

#include "Test.h"
#include <unistd.h>

#include "libavutil/log.h"
#include "libavformat/avformat.h"
#include "libavdevice/avdevice.h"

int test(void) {

    av_log_set_level(AV_LOG_INFO);
    avdevice_register_all();/// 注册设备

    AVFormatContext *fmt_ctx = NULL;
    const char *url = "/Users/i7y/Desktop/Demo.mov";
    AVDictionary *dict = NULL;
    /// 第三个参数传递 NULL，根据 url 解析输入文件
    int ret = avformat_open_input(&fmt_ctx, url, NULL, &dict);
    if (ret < 0) {
        av_log(NULL, AV_LOG_ERROR, "Can't open inputFile : %s\n",av_err2str(ret));
        return -1;
    }

    /// 参数2 ： 流的索引。 参数4 ：输出流/输入流
    av_dump_format(fmt_ctx, 0, url, 0);

    /// 拿到最好的流
    ret = av_find_best_stream(fmt_ctx, AVMEDIA_TYPE_AUDIO, -1, -1, NULL, 0);
    if (ret < 0) {
        av_log(NULL, AV_LOG_ERROR, "Can't open best stream : %s\n",av_err2str(ret));
        avformat_close_input(&fmt_ctx);
        return -2;
    }
    int stream_index = ret;

    const char *outURL = "/Users/i7y/Desktop/Demo.aac";
    FILE *file = fopen(outURL, "wb+");
    if (file == NULL) {
        av_log(NULL, AV_LOG_ERROR, "Can't open outFile\n");
        avformat_close_input(&fmt_ctx);
        return -3;
    }

    AVPacket pkt;
    av_init_packet(&pkt);

    int length = 0;
    while ((ret = av_read_frame(fmt_ctx, &pkt)) >= 0 || ret == -35) {
        if (ret == -35) {
            usleep(1);
            continue;
        }

        if (pkt.stream_index == stream_index) {
            length = fwrite(pkt.data, 1, pkt.size, file);
            if (length != pkt.size) {
                av_log(NULL, AV_LOG_WARNING, "warning, length of writed data isn't equal pkt.size(%d, %d)\n", length, pkt.size);
            }
        }
        av_packet_unref(&pkt);
    }

    avformat_close_input(&fmt_ctx);
    fclose(file);
    return 0;
}
