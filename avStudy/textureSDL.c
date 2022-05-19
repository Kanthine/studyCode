#include <SDL.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
    int quit = 1;
    SDL_Event event;
	SDL_Window *window = NULL;
	SDL_Renderer *render = NULL;
    SDL_Texture *texture = NULL;
    
    SDL_Rect rect; /// 矩形
    rect.w = 30;
    rect.h = 30;
    
	SDL_Init(SDL_INIT_VIDEO);
	window = SDL_CreateWindow("SDL2 Window",
                              300, 300,
                              640, 480,
                              SDL_WINDOW_SHOWN);
	if(!window){
		printf("Failed to creat window!\n");
		goto __EXIT;
	}

	render = SDL_CreateRenderer(window, -1, 0);
	if(!render){
		SDL_Log("Failed to create Render!");
		goto __FREERENDER;
	}
	
    texture = SDL_CreateTexture(render, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 640, 480);
    if(!texture){
        SDL_Log("Failed to create texture!");
        goto __FREETEXTURE;
    }
    
	do {
		SDL_PollEvent(&event);
		switch (event.type) {
            case SDL_QUIT: {
				quit = 0;
			}break;
			default: {
				SDL_Log("event type is %d", event.type);
			}break;
		}
        

        rect.x = rand() % 640;
        rect.y = rand() % 480;
        
        /// 1、改变 RenderTarget，将整个纹理刷成透明色
        SDL_SetRenderTarget(render, texture);
        SDL_SetRenderDrawColor(render, 0, 0, 0, 0);
        SDL_RenderClear(render);    /// 整个窗口

        /// 2、绘制矩形，设置矩形颜色并填充到矩形中；
        SDL_RenderDrawRect(render, &rect);
        SDL_SetRenderDrawColor(render, 255, 0, 0, 255);
        SDL_RenderFillRect(render, &rect); /// 矩形范围
        
        /// 3、将纹理输出到窗口：改变渲染目标、将纹理拷贝到窗口
        SDL_SetRenderTarget(render, NULL);
        SDL_RenderCopy(render, texture, NULL, NULL);
        
        /// 4、显示渲染结果
        SDL_RenderPresent(render);
    } while(quit);
    
    
__FREETEXTURE:
    SDL_DestroyTexture(texture);
__FREERENDER:
	SDL_DestroyWindow(window);
    SDL_DestroyRenderer(render);
__EXIT:
	SDL_Quit();
	return 0; 
}
