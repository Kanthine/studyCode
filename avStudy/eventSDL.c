#include <SDL.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
    int quit = 1;
    SDL_Event event;
	SDL_Window *window = NULL;
	SDL_Renderer *render = NULL;
    
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
    SDL_SetRenderDrawColor(render, 255, 0, 0, 255);
    SDL_RenderClear(render);
	SDL_RenderPresent(render);

    do {
		SDL_WaitEvent(&event);
		switch (event.type) {
            case SDL_QUIT: {
				quit = 0;
			}break;
			default: {
				SDL_Log("event type is %d", event.type);
			}break;
		}
    } while(quit);

__FREERENDER:
	SDL_DestroyWindow(window);
    SDL_DestroyRenderer(render);
__EXIT:
	SDL_Quit();
	return 0; 
}
