#pragma once

#include <string>
#include <SDL.h>
#include "Level.h"

class Renderer
{
private:
	Renderer(const std::string WindowTitle, const int w, const int h);
	~Renderer();
	static Renderer* this_Renderer;

public:
	static Renderer* GetRenderer(const std::string WindowTitle, const int w, const int h);
	static void DestroyRenderer();

public:
	void Render(const Level* TargetLevel);
	SDL_Renderer* GetSDLRenderer();

private:
	SDL_Window* m_Window;
	SDL_Renderer* m_Renderer;
};