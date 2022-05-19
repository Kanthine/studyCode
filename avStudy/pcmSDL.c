#include <SDL.h>
#include <stdio.h>

#define BLOCK_SIZE 4096000 /// 4M大小

static Uint8 *audio_buf = NULL; /// 将音频文件读取到该缓存
static Uint8 *audio_pos = NULL; ///
static size_t buf_len = 0;      /// 每次从多媒体文件实际读取的数据大小

void read_audio_data(void *udata, Uint8 *stream, int len) {
    /// audio_buf 中没有数据
    if (buf_len == 0) return;
    
    /// 清空 stream，防止遗留数据造成声音质量损失
    SDL_memset(stream, 0, len);
    
    /// 计算读取的数据大小：如果 buf_len < len, 则只能读取 buf_len 大小的数据；
    len = (len < buf_len) ? len : buf_len;
    
    /// 从 audio_pos 拷贝长度为 len 的数据到 stream
    /// 音量大小为最大音量 SDL_MIX_MAXVOLUME
    SDL_MixAudio(stream, audio_pos, len, SDL_MIX_MAXVOLUME);
     
    /// 重置 audio_pos
    audio_pos += len;
    buf_len -= len;
}

int main(int argc, char *argv[]){
    char *audio_path = "/Users/i7y/Desktop/2.pcm";
    FILE *audio_file = NULL;
    
    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER)) {
        SDL_Log("Failed to init");
        return -1;
    }
    
    /// 打开文件
    audio_file = fopen(audio_path, "rb");
    if (!audio_file) {
        SDL_Log("Failed to open audio file");
        goto __FAIL;
    }
    
    /// 开辟缓存：将文件数据读取到缓存中
    audio_buf = (Uint8 *)malloc(BLOCK_SIZE);
    if(audio_buf == NULL) {
        SDL_Log("Failed to alloc audio buffer");
        goto __FAIL;
    }
    
    /// 打开音频设备：根据指定参数
    SDL_AudioSpec spec;
    spec.freq = 44100; /// 采样率
    spec.channels = 2; /// 声道数
    spec.format = AUDIO_S16SYS; /// 采样大小
    spec.silence = 0;
    spec.samples = 1024;
    spec.callback = read_audio_data;/// 回调函数
    spec.userdata = NULL; /// 回调参数
    if (SDL_OpenAudio(&spec, NULL)) {
        SDL_Log("Fail to open audio device");
        goto __FAIL;
    }
    
    /// 0 播放 ；1 暂停
    SDL_PauseAudio(0);
    
    /// 从 PCM 读取数据到 audio_buf
    /// 一共有两个 buffer ：
    /// 程序开辟的 audio_buf：读取的 PCM 数据存放；
    /// 打开声卡自带的 buffer：大小由采样率、声道数、菜样大小决定；
    /// 声卡 buf 可能仅有几 K 大小，需要时从 audio_buf 读取数据；
    ///
    do {
        /// 从 audio_file 读取 1 块 BLOCK_SIZE 大小的数据到 audio_buf
        buf_len = fread(audio_buf, 1, BLOCK_SIZE, audio_file);
        audio_pos = audio_buf;
        
        /// 如果指针在 audio_buf 头部与尾部之间；
        /// 证明 audio_buf 之中还有未被声卡消费完的数据；
        while (audio_pos < (audio_buf + buf_len)) {
            /// 暂停一会，让声卡 buf 消费掉 audio_buf 的数据
            SDL_Delay(1);
        }
        
        /// 声卡将 audio_buf 的数据消费完毕，audio_buf 再次从多媒体文件填充数据
    } while(buf_len != 0); /// 一直读取到文件尾部停止
    
    SDL_CloseAudio();
    
__FAIL:
    if(audio_buf) free(audio_buf);
    if (audio_file) fclose(audio_file);
    SDL_Quit();
    
	return 0;
}
