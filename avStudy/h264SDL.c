#include <stdio.h>
#include <SDL.h>

#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>

// 兼容新的API
#if LIBAVCODEC_VERSION_INT < AV_VERSION_INT(55,28,1)
#define av_frame_alloc avcodec_alloc_frame
#define av_frame_free avcodec_free_frame
#endif

int main(int argc, char *argv[]) {
    
    if(argc < 2) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Usage: command <file>");
        return ret;
    }
    const char *input_file = argv[1];
    
    /****************** 变量定义 *******************/
    int ret = -1;

    AVFormatContext *pFormatCtx = NULL; /// 多媒体文件上下文

    int             i, videoStream;

    AVCodecContext  *pCodecCtxOrig = NULL; /// 编解码上下文
    AVCodecContext  *pCodecCtx = NULL;
    
    struct SwsContext *sws_ctx = NULL; /// 视频裁剪上下文

    AVCodec         *pCodec = NULL; /// 编解码器
    AVFrame         *pFrame = NULL; /// 解码后的数据帧
    AVPacket        packet; /// 解码前的数据包

    int             frameFinished;
    float           aspect_ratio;

    AVPicture        *pict  = NULL; /// 存放解码出的 YUV 数据

    SDL_Rect        rect; /// 渲染窗口大小
    Uint32       pixformat;

    SDL_Window       *win = NULL;     /// 渲染窗口
    SDL_Renderer    *renderer = NULL; /// 渲染器
    SDL_Texture     *texture = NULL;  /// 纹理

    /****************** 变量初始化 *******************/

    //set defualt size of window
    int w_width = 640;
    int w_height = 480;

    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER)) {
        //fprintf(stderr, "Could not initialize SDL - %s\n", SDL_GetError());
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Could not initialize SDL - %s\n", SDL_GetError());
        return ret;
    }

    //Register all formats and codecs
    av_register_all();

    // 打开多媒体文件
    if(avformat_open_input(&pFormatCtx, input_file, NULL, NULL) != 0){
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to open video file!");
        goto __FAIL; // Couldn't open file
    }
  
    // 查找流信息
    if(avformat_find_stream_info(pFormatCtx, NULL)<0){
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to find stream infomation!");
        goto __FAIL; // Couldn't find stream information
    }
  
    // Dump information about file onto standard error
    av_dump_format(pFormatCtx, 0, input_file, 0);
    
    // 查找视频流在多媒体文件中的索引（多媒体文件中有多路流）
    videoStream = -1;
    for(i = 0; i < pFormatCtx->nb_streams; i++) {
        if(pFormatCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO) {
            videoStream = i;
            break;
        }
    }
    if(videoStream == -1){ /// 没有发现视频流
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Din't find a video stream!");
        goto __FAIL;// Didn't find a video stream
    }
  
    /// 根据视频流，拿到对应的编解码器上下文
    pCodecCtxOrig = pFormatCtx->streams[videoStream]->codec;

    /// 根据编解码器上下文，拿到对应的解码器
    pCodec = avcodec_find_decoder(pCodecCtxOrig->codec_id);
    if(pCodec == NULL) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Unsupported codec!\n");
        goto __FAIL; // Codec not found
    }

    /// 为不破坏源数据，拷贝一份
    pCodecCtx = avcodec_alloc_context3(pCodec);
    if(avcodec_copy_context(pCodecCtx, pCodecCtxOrig) != 0) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,  "Couldn't copy codec context");
        goto __FAIL;// Error copying codec context
    }

    // 打开解码器
    if(avcodec_open2(pCodecCtx, pCodec, NULL)<0) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to open decoder!\n");
        goto __FAIL; // Could not open codec
    }
 
    // /解码后的数据帧
    pFrame = av_frame_alloc();

    /// 渲染窗口
    w_width = pCodecCtx->width;
    w_height = pCodecCtx->height;
    win = SDL_CreateWindow( "Media Player",
                           SDL_WINDOWPOS_UNDEFINED,
                           SDL_WINDOWPOS_UNDEFINED,
                           w_width, w_height,
                           SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    if(!win){
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to create window by SDL");
        goto __FAIL;
    }
    
    /// 渲染器
    renderer = SDL_CreateRenderer(win, -1, 0);
    if(!renderer){
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to create Renderer by SDL");
        goto __FAIL;
    }
    
    /// 渲染纹理
    pixformat = SDL_PIXELFORMAT_IYUV;
    texture = SDL_CreateTexture(renderer,
                                pixformat,
                                SDL_TEXTUREACCESS_STREAMING,/// 视频操作，持续的流数据
                                w_width,
                                w_height);

    /// 图像裁剪相关上下文
    /// 如果需要裁剪，可以定义区别于原始宽高的目的宽高
    sws_ctx = sws_getContext(pCodecCtx->width, /// 原始宽高
                             pCodecCtx->height,
                             pCodecCtx->pix_fmt,
                             pCodecCtx->width, /// 目的宽高
                             pCodecCtx->height,
                             AV_PIX_FMT_YUV420P,
                             SWS_BILINEAR,
                             NULL,
                             NULL,
                             NULL);

    pict = (AVPicture*)malloc(sizeof(AVPicture));
    avpicture_alloc(pict,
                    AV_PIX_FMT_YUV420P,
                    pCodecCtx->width,
                    pCodecCtx->height);


    /// 打开多媒体文件后，就可以读取一帧一帧数据（压缩前的数据）
    while(av_read_frame(pFormatCtx, &packet)>=0) {
        /// 在 packet 数据中查找与视频流相关的流
        if(packet.stream_index == videoStream) {
            // 解码视频帧
            avcodec_decode_video2(pCodecCtx, pFrame, &frameFinished, &packet);
            if(frameFinished) { // 解码成功

                /// 将解码后的图像转换成 SDL 使用的YUV格式
                sws_scale(sws_ctx, (uint8_t const * const *)pFrame->data,
                          pFrame->linesize, 0, pCodecCtx->height,
                          pict->data, pict->linesize);

                SDL_UpdateYUVTexture(texture, NULL,
                                     pict->data[0], pict->linesize[0], /// Y
                                     pict->data[1], pict->linesize[1], /// U
                                     pict->data[2], pict->linesize[2]); /// V

                // Set Size of Window
                rect.x = 0;
                rect.y = 0;
                rect.w = pCodecCtx->width;
                rect.h = pCodecCtx->height;
                
                SDL_RenderClear(renderer);
                SDL_RenderCopy(renderer, texture, NULL, &rect);
                SDL_RenderPresent(renderer);
            }
        }

        // Free the packet that was allocated by av_read_frame
        av_free_packet(&packet);

        SDL_Event event;
        SDL_PollEvent(&event);
        switch(event.type) {
            case SDL_QUIT:
                goto __QUIT;
                break;
            default:
                break;
        }
    }

__QUIT:
    ret = 0;
  
__FAIL:
    // Free the YUV frame
    if(pFrame){
        av_frame_free(&pFrame);
    }
  
    // Close the codec
    if(pCodecCtx){
        avcodec_close(pCodecCtx);
    }

    if(pCodecCtxOrig){
        avcodec_close(pCodecCtxOrig);
    }
  
    // Close the video file
    if(pFormatCtx){
      avformat_close_input(&pFormatCtx);
    }

    if(pict){
        avpicture_free(pict);
        free(pict);
    }

    if(win){
        SDL_DestroyWindow(win);
    }

    if(renderer){
        SDL_DestroyRenderer(renderer);
    }

    if(texture){
        SDL_DestroyTexture(texture);
    }

    SDL_Quit();
    return ret;
}
