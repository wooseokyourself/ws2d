#include "Renderer.h"

SDL_Renderer* InitRenderer(const std::string WindowName, int w, int h)
{
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_Window* Window = SDL_CreateWindow(WindowName.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w, h, SDL_WINDOW_SHOWN);
	return SDL_CreateRenderer(Window, -1, 0);
}