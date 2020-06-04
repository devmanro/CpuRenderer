#include <SDL.h>
#include "renderer.h"
#include <cstdlib>

using CPURenderer::ViewPort;

int main(int argc, char *argv[]) {
	SDL_Event event;
	SDL_Renderer *renderer;
	SDL_Window *window;
	SDL_Texture* texture;

	SDL_Init(SDL_INIT_VIDEO);
	window = SDL_CreateWindow("CPU Renderer", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, ViewPort::instance.width, ViewPort::instance.height, 0);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	texture = SDL_CreateTexture(renderer,
		SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STATIC, ViewPort::instance.width, ViewPort::instance.height);

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_RenderClear(renderer);

	CPURenderer::Renderer r;

	while (1) {
		if (SDL_PollEvent(&event) && event.type == SDL_QUIT)
			break;

		r.render_loop();
		SDL_UpdateTexture(texture, NULL, ViewPort::instance.frame_buffer, ViewPort::instance.width * 4 * sizeof(unsigned char));
		SDL_RenderCopy(renderer, texture, NULL, NULL);
		SDL_RenderPresent(renderer);
	}
	SDL_DestroyTexture(texture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}