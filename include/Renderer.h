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
	static void Destroy();

public:
	void Render(Level* TargetLevel, Camera* View);

private:
	SDL_Window* m_Window;
	SDL_Renderer* m_Renderer;
};