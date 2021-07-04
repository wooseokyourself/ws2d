#pragma once

#include <SDL.h>

class Camera
{
public:
	Camera(const int w, const int h)
	{
		m_Rect.x = 0;
		m_Rect.y = 0;
		m_Rect.w = w;
		m_Rect.h = h;
	}
	void FocusOn(const int x, const int y)
	{
		m_Rect.x = x - m_Rect.w / 2;
		m_Rect.y = y - m_Rect.h / 2;
	}
	SDL_Rect GetRect() const
	{
		return m_Rect;
	}

public:
	SDL_Rect m_Rect;
};