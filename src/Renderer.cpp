#include "Renderer.h"

Renderer::Renderer(const std::string WindowTitle, const int w, const int h)
{
	SDL_Init(SDL_INIT_EVERYTHING);
	m_Window = SDL_CreateWindow(WindowTitle.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w, h, SDL_WINDOW_SHOWN);
	m_Renderer = SDL_CreateRenderer(m_Window, -1, 0);
}

Renderer::~Renderer()
{
	SDL_DestroyRenderer(m_Renderer);
	SDL_DestroyWindow(m_Window);
}

Renderer* Renderer::GetRenderer(const std::string WindowTitle, const int w, const int h)
{
	if (!this_Renderer)
		this_Renderer = new Renderer(WindowTitle, w, h);
	return this_Renderer;
}

void Renderer::DestroyRenderer()
{
	if (!this_Renderer)
		return;
	delete this_Renderer;
	this_Renderer = nullptr;
}

void Renderer::Render(const Level* TargetLevel)
{
	SDL_RenderClear(m_Renderer);
	TargetLevel->Draw(m_Renderer);
	SDL_RenderPresent(m_Renderer);
}

SDL_Renderer* Renderer::GetSDLRenderer()
{
	return m_Renderer;
}