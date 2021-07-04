#pragma once

#include <SDL_events.h>
#include <b2_math.h>

class Interface
{
public:
private:
	static Interface* this_Interface;
	
public:
	static Interface* GetInterface()
	{
		if (!this_Interface)
			this_Interface = new Interface();
		return this_Interface;
	}
	static void Destroy()
	{
		if (!this_Interface)
			return;
		delete this_Interface;
		this_Interface = nullptr;
	}

private:
	void Clear()
	{
		m_MouseLeftClickedPosition.x = -1;
		m_MouseLeftClickedPosition.y = -1;
		m_MouseRightClickedPosition.x = -1;
		m_MouseRightClickedPosition.y = -1;
	}
	void MouseButtonUp(SDL_MouseButtonEvent& Event)
	{
		switch (Event.button)
		{
		case SDL_BUTTON_LEFT:
		{
			m_MouseLeftClickedPosition.x = Event.x;
			m_MouseLeftClickedPosition.y = Event.y;
			break;
		}
		case SDL_BUTTON_RIGHT:
		{
			m_MouseRightClickedPosition.x = Event.x;
			m_MouseRightClickedPosition.y = Event.y;
			break;
		}
		}
	}

public:
	void Input()
	{
		Clear();
		while (SDL_PollEvent(&m_Event))
		{
			switch (m_Event.type)
			{
			case SDL_MOUSEMOTION:
				break;
			case SDL_MOUSEBUTTONUP:
			{
				MouseButtonUp(m_Event.button);
				break;
			}
			case SDL_KEYDOWN:
				m_ContinuousKeyboardTable[m_Event.key.keysym.sym] = true;
				break;
			case SDL_KEYUP:
				m_ContinuousKeyboardTable[m_Event.key.keysym.sym] = false;
			}
		}
	}
	inline bool IsHeldDown(const int SDL_KEYCODE)
	{
		return m_ContinuousKeyboardTable[SDL_KEYCODE];
	}
	inline bool IsPressedOnce(const int SDL_KEYCODE) // Maybe modify nexttime.
	{
		return m_ContinuousKeyboardTable[SDL_KEYCODE];
	}
	inline b2Vec2 GetMousePosition()
	{
		return m_MousePosition;
	}
	inline b2Vec2 GetMouseLeftClickedPosition()
	{
		return m_MouseLeftClickedPosition;
	}
	inline b2Vec2 GetMouseRightClickedPosition()
	{
		return m_MouseRightClickedPosition;
	}

private:
	SDL_Event m_Event;
	bool m_ContinuousKeyboardTable[1 << 30];
	b2Vec2 m_MousePosition;
	b2Vec2 m_MouseLeftClickedPosition;
	b2Vec2 m_MouseRightClickedPosition;
};

Interface* Interface::this_Interface = NULL;