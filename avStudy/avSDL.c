#include <stdio.h>
#include <assert.h>

#include <SDL.h>

#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libswresample/swresample.h>

// 兼容新的 API
#if LIBAVCODEC_VERSION_INT < AV_VERSION_INT(55,28,1)
#define av_frame_alloc avcodec_alloc_frame
#define av_frame_free avcodec_free_frame
#endif

#define SDL_AUDIO_BUFFER_SIZE 1024
#define MAX_AUDIO_FRAME_SIZE 192000

struct SwrContext *audio_convert_ctx = NULL;

typedef struct PacketQueue {
    AVPacketList *first_pkt, *last_pkt;
    int nb_packets;
    int size;
    SDL_mutex *mutex;
    SDL_cond *cond;
} PacketQueue;

PacketQueue audioq;

int quit = 0;

void packet_queue_init(PacketQueue *q) {
    memset(q, 0, sizeof(PacketQueue));
    q->mutex = SDL_CreateMutex();
    q->cond = SDL_CreateCond();
}

int packet_queue_put(PacketQueue *q, AVPacket *pkt) {

    AVPacketList *pkt1;
    if(av_dup_packet(pkt) < 0) return -1;
    pkt1 = av_malloc(sizeof(AVPacketList));
    if (!pkt1) return -1;
          
    pkt1->pkt = *pkt;
    pkt1->next = NULL;
      
    SDL_LockMutex(q->mutex);
      
    if (!q->last_pkt) {
        q->first_pkt = pkt1;
    }else{
        q->last_pkt->next = pkt1;
    }

    q->last_pkt = pkt1;
    q->nb_packets++;
    q->size += pkt1->pkt.size;
    SDL_CondSignal(q->cond);

    SDL_UnlockMutex(q->mutex);
    return 0;
}

int packet_queue_get(PacketQueue *q, AVPacket *pkt, int block) {
    AVPacketList *pkt1;
    int ret;

    SDL_LockMutex(q->mutex);

    for(;;) {
    
        if(quit) {
            ret = -1;
            break;
        }

        pkt1 = q->first_pkt;
        if (pkt1) {
            q->first_pkt = pkt1->next;
            if (!q->first_pkt) q->last_pkt = NULL;
            q->nb_packets--;
            q->size -= pkt1->pkt.size;
            *pkt = pkt1->pkt;
            av_free(pkt1);
            ret = 1;
            break;
        } else if (!block) {
            ret = 0;
            break;
        } else {
            SDL_CondWait(q->cond, q->mutex);
        }
    }
    SDL_UnlockMutex(q->mutex);
    return ret;
}

/// 音频解码函数
int audio_decode_frame(AVCodecContext *aCodecCtx, uint8_t *audio_buf, int buf_size) {

    static AVPacket pkt;
    static uint8_t *audio_pkt_data = NULL;
    static int audio_pkt_size = 0;
    static AVFrame frame;

    int len1, data_size = 0;

    for(;;) {
        
        while(audio_pkt_size > 0) { /// 当队列有未解码的数据
            int got_frame = 0;
            len1 = avcodec_decode_audio4(aCodecCtx, &frame, &got_frame, &pkt); /// 解码
            if(len1 < 0) {  /// 解码失败
                audio_pkt_size = 0;
                break;
            }
            audio_pkt_data += len1;
            audio_pkt_size -= len1;
            data_size = 0;
            if(got_frame) {
            /*
            data_size = av_samples_get_buffer_size(NULL,
                           aCodecCtx->channels,
                           frame.nb_samples,
                           aCodecCtx->sample_fmt,
                           1);
             */
                data_size = 2 * 2 * frame.nb_samples;
                assert(data_size <= buf_size);
                
                /// 将多媒体声音转为声卡可以识别的声音
                swr_convert(audio_convert_ctx,
                            &audio_buf,
                            MAX_AUDIO_FRAME_SIZE*3/2,
                            (const uint8_t **)frame.data,
                            frame.nb_samples);

                //memcpy(audio_buf, frame.data[0], data_size);
            }
            if(data_size <= 0) {
                /* No data yet, get more frames */
                continue;
            }
            /* We have data, return it and come back for more later */
            return data_size;
        }
        if(pkt.data)  av_free_packet(&pkt);
        if(quit) return -1;
        
        /// 不停的从队列中：获取音频包、解码、输出
        if(packet_queue_get(&audioq, &pkt, 1) < 0) return -1;
        audio_pkt_data = pkt.data;
        audio_pkt_size = pkt.size;
    }
}

/** 声卡回调函数
 * @param userdata 用户数据
 * @param stream 声卡需要的缓冲区地址
 * @param len 声卡需要的 buf 长度
 */
void audio_callback(void *userdata, Uint8 *stream, int len) {
    AVCodecContext *aCodecCtx = (AVCodecContext *)userdata; /// 音频编解码上下文
    int len1, audio_size;
    
    static uint8_t audio_buf[(MAX_AUDIO_FRAME_SIZE * 3) / 2];
    static unsigned int audio_buf_size = 0;
    static unsigned int audio_buf_index = 0;

    while(len > 0) { /// 如果声卡缓冲区还没填满
        if(audio_buf_index >= audio_buf_size) {
            
            /// 音频解码
            audio_size = audio_decode_frame(aCodecCtx, audio_buf, sizeof(audio_buf));
            if(audio_size < 0) { /// 解码失败
                /// 为保证声音是连续的，需要输出一段静默音
                audio_buf_size = 1024;
                memset(audio_buf, 0, audio_buf_size);
                /// 将静默音输出到声卡缓冲区，保证声卡一直在工作
            } else { /// 解码成功
                audio_buf_size = audio_size; /// 获取解码后的缓冲区大小
            }
            audio_buf_index = 0;
        }
        len1 = audio_buf_size - audio_buf_index;
        if(len1 > len) len1 = len;
        fprintf(stderr, "index=%d, len1=%d, len=%d\n", audio_buf_index, len, len1);
        /// 将 audio_buf 的数据拷贝到 stream 中
        memcpy(stream, (uint8_t *)audio_buf + audio_buf_index, len1);
        len -= len1;
        stream += len1;
        audio_buf_index += len1;
    }
}

int main(int argc, char *argv[]) {

    /****************** 变量定义 *******************/
    int ret = -1;
    int i, videoStream, audioStream;

    AVFormatContext *pFormatCtx = NULL; /// 多媒体文件上下文

    // 视频解码
    AVCodecContext  *pCodecCtxOrig = NULL;
    AVCodecContext  *pCodecCtx = NULL;
    AVCodec         *pCodec = NULL; /// 编解码器

    struct SwsContext *sws_ctx = NULL; /// 视频裁剪上下文

    AVPicture *pict   = NULL;   /// 存放解码出的 YUV 数据
    AVFrame   *pFrame = NULL;   /// 解码后的数据帧
    AVPacket  packet;           /// 解码前的数据包
    int       frameFinished;

    // 音频解码
    AVCodecContext  *aCodecCtxOrig = NULL;
    AVCodecContext  *aCodecCtx = NULL;
    AVCodec         *aCodec = NULL;
    /// 音频的输入输出声道
    int64_t in_channel_layout;
    int64_t out_channel_layout;

    //for video render
    int w_width = 640;
    int w_height = 480;

    int             pixformat;
    SDL_Rect        rect;

    SDL_Window      *win;
    SDL_Renderer    *renderer;
    SDL_Texture     *texture;

    // SDL 事件
    SDL_Event       event;

    // 音频描述
    SDL_AudioSpec   wanted_spec, spec;

    if(argc < 2) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Usage: command <file>");
        return ret;
    }

    /****************** 变量初始化 *******************/

    // Register all formats and codecs
    av_register_all();
  
    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER)) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Could not initialize SDL - %s\n", SDL_GetError());
        return ret;
    }

    // 查找流信息
    if(avformat_open_input(&pFormatCtx, argv[1], NULL, NULL)!=0) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to open multi-media file");
        goto __FAIL; // Couldn't open file
    }
  
    // 寻找视频流
    if(avformat_find_stream_info(pFormatCtx, NULL) < 0) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't find stream information ");
        goto __FAIL;
    }

    // Dump information about file onto standard error
    av_dump_format(pFormatCtx, 0, argv[1], 0);
    
    // 查找音、视频流在多媒体文件中的索引（多媒体文件中有多路流）
    videoStream = -1;
    audioStream = -1;
    for(i = 0; i < pFormatCtx->nb_streams; i++) {
        if(pFormatCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO &&
           videoStream < 0) {
            videoStream = i;
        }
        if(pFormatCtx->streams[i]->codec->codec_type==AVMEDIA_TYPE_AUDIO &&
           audioStream < 0) {
            audioStream = i;
        }
    }
    if(videoStream==-1) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, " Didn't find a video stream ");
        goto __FAIL; // Didn't find a video stream
    }
    if(audioStream==-1) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, " Didn't find a audio stream ");
        goto __FAIL; // Didn't find a video stream
    }
   
    /// 根据音频流，拿到对应的编解码器上下文
    aCodecCtxOrig=pFormatCtx->streams[audioStream]->codec;
    /// 根据编解码器上下文，拿到对应的解码器
    aCodec = avcodec_find_decoder(aCodecCtxOrig->codec_id);
    if(!aCodec) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Unsupported codec! ");
        goto __FAIL; // Didn't find a video stream
    }
    
    /// 为不破坏源数据，拷贝一份
    aCodecCtx = avcodec_alloc_context3(aCodec);
    if(avcodec_copy_context(aCodecCtx, aCodecCtxOrig) != 0) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't copy codec context! ");
        goto __FAIL; // Didn't find a video stream
    }

    /// 设置音频编码参数
    wanted_spec.freq = aCodecCtx->sample_rate;  /// 采样率
    wanted_spec.format = AUDIO_S16SYS;          /// 采样格式
    wanted_spec.channels = aCodecCtx->channels; /// 声道数
    wanted_spec.silence = 0;                    /// 是否有静默音
    wanted_spec.samples = SDL_AUDIO_BUFFER_SIZE;/// 采样大小
    wanted_spec.callback = audio_callback;      /// 回调函数
    wanted_spec.userdata = aCodecCtx;           /// 传入参数
    
    if(SDL_OpenAudio(&wanted_spec, &spec) < 0) { /// SDL 打开音频设备
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to open audio device - %s!", SDL_GetError());
        goto __FAIL;
    }

    // 打开解码器
    avcodec_open2(aCodecCtx, aCodec, NULL);
    packet_queue_init(&audioq); /// 初始化音频队列

    in_channel_layout = av_get_default_channel_layout(aCodecCtx->channels);
    out_channel_layout = in_channel_layout; //AV_CH_LAYOUT_STEREO;
    fprintf(stderr, "in layout:%lld, out layout:%lld \n", in_channel_layout, out_channel_layout);
    
    audio_convert_ctx = swr_alloc();
    if(audio_convert_ctx){ /// 输入的音频数据格式转换
        swr_alloc_set_opts(audio_convert_ctx,
                           out_channel_layout,
                           AV_SAMPLE_FMT_S16,
                           aCodecCtx->sample_rate,
                           in_channel_layout,
                           aCodecCtx->sample_fmt,
                           aCodecCtx->sample_rate,
                           0,
                           NULL);
    }
    swr_init(audio_convert_ctx);
    SDL_PauseAudio(0); /// 开启声卡
    
    /// 根据视频流，拿到对应的编解码器上下文
    pCodecCtxOrig=pFormatCtx->streams[videoStream]->codec;
    /// 根据编解码器上下文，拿到对应的解码器
    pCodec=avcodec_find_decoder(pCodecCtxOrig->codec_id);
    if(pCodec==NULL) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Unsupported codec!");
        goto __FAIL;
    }

    /// 为不破坏源数据，拷贝一份
    pCodecCtx = avcodec_alloc_context3(pCodec);
    if(avcodec_copy_context(pCodecCtx, pCodecCtxOrig) != 0) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to copy context of codec!");
        goto __FAIL;
    }

    // 打开解码器
    if(avcodec_open2(pCodecCtx, pCodec, NULL)<0) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to open audio decoder!");
        goto __FAIL;
    }
  
    // /解码后的数据帧
    pFrame = av_frame_alloc();
    
    /// 渲染窗口
    w_width = pCodecCtx->width;
    w_height = pCodecCtx->height;
    fprintf(stderr, "width:%d, height:%d\n", w_width, w_height);
    win = SDL_CreateWindow("Media Player",
                           SDL_WINDOWPOS_UNDEFINED,
                           SDL_WINDOWPOS_UNDEFINED,
                           w_width, w_height,
                           SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    if(!win){
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to create window!");
        goto __FAIL;
    }

    /// 渲染器
    renderer = SDL_CreateRenderer(win, -1, 0);
    if(!renderer){
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to create renderer!");
        goto __FAIL;
    }

    /// 渲染纹理
    pixformat = SDL_PIXELFORMAT_IYUV;
    texture = SDL_CreateTexture(renderer,
                                pixformat,
                                SDL_TEXTUREACCESS_STREAMING,
                                w_width,
                                w_height);
    if(!texture){
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to create Texture!");
        goto __FAIL;
    }
  
    /// 图像裁剪相关上下文
    /// 如果需要裁剪，可以定义区别于原始宽高的目的宽高
    sws_ctx = sws_getContext(pCodecCtx->width,
                             pCodecCtx->height,
                             pCodecCtx->pix_fmt,
                             pCodecCtx->width,
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
                                     pict->data[0], pict->linesize[0],
                                     pict->data[1], pict->linesize[1],
                                     pict->data[2], pict->linesize[2]);

                rect.x = 0;
                rect.y = 0;
                rect.w = pCodecCtx->width;
                rect.h = pCodecCtx->height;

                SDL_RenderClear(renderer);
                SDL_RenderCopy(renderer, texture, NULL, &rect);
                SDL_RenderPresent(renderer);

                av_free_packet(&packet);
            }
        } else if(packet.stream_index==audioStream) { /// 在 packet 数据中查找与音频流相关的流
            packet_queue_put(&audioq, &packet); /// 将音频包存储在队列中
        } else {
            av_free_packet(&packet);
        }

        // Free the packet that was allocated by av_read_frame
        SDL_PollEvent(&event);
        switch(event.type) {
            case SDL_QUIT:
                quit = 1;
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
  
  // Close the codecs
  if(pCodecCtxOrig){
    avcodec_close(pCodecCtxOrig);
  }

  if(pCodecCtx){
    avcodec_close(pCodecCtx);
  }
 
  if(aCodecCtxOrig) {
    avcodec_close(aCodecCtxOrig);
  }

  if(aCodecCtx) {
    avcodec_close(aCodecCtx);
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
