#include <stdio.h>
#include "libavdevice/avdevice.h"
#include "libavformat/avformat.h"
#include "libavutil/log.h"


int main(int argc, char *argv[]) {
    
    if(argc < 2) {
        av_log(NULL, AV_LOG_ERROR, "Usage: command <file>");
        return 0;
    }
    const char *url = argv[1];
    
    av_log_set_level(AV_LOG_INFO);
    avdevice_register_all();/// 注册设备
    
    AVFormatContext *fmt_ctx = NULL;
    AVDictionary *dict = NULL;
    /// 第三个参数传递 NULL，根据 url 解析输入文件
    int ret = avformat_open_input(&fmt_ctx, url, NULL, &dict);
    if (ret < 0) {
        av_log(NULL, AV_LOG_ERROR, "Can't open file : %s\n",av_err2str(ret));
        return 0;
    }
    
    /// 参数2 ： 流的索引
    /// 参数4 ：输出流/输入流
    av_dump_format(fmt_ctx, 0, url, 0);
    avformat_close_input(&fmt_ctx);
    
    return 0;
}
