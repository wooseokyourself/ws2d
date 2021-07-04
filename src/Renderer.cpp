#include "Renderer.h"

Renderer* Renderer::this_Renderer = NULL;

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

void Renderer::Render(Level* TargetLevel, Camera* View)
{
	SDL_RenderClear(m_Renderer);

	if (View->m_Rect.x < 0)
		View->m_Rect.x = 0;
	else if (View->m_Rect.x + View->m_Rect.w > TargetLevel->w)
		View->m_Rect.x = TargetLevel->w - View->m_Rect.w;
	if (View->m_Rect.y < 0)
		View->m_Rect.y = 0;
	else if (View->m_Rect.y + View->m_Rect.h > TargetLevel->h)
		View->m_Rect.y = TargetLevel->h - View->m_Rect.h;

	SDL_Rect CameraRect = View->GetRect();
	for (Object* Target : TargetLevel->m_Objects)
	{
		SDL_Rect DstRect = Target->m_WorldBox;
		DstRect.x -= CameraRect.x;
		DstRect.y -= CameraRect.y;
		if (DstRect.x > CameraRect.w || DstRect.x + DstRect.w < 0
			|| DstRect.y > CameraRect.h || DstRect.y + DstRect.h < 0)
			continue;
		else // Rendering
		{
			if (!Target->m_Texture)
				Target->SetTexture(m_Renderer);
			SDL_Point p; p.x = 0; p.y = 0; // rotate axis
			SDL_RenderCopyEx(m_Renderer, Target->m_Texture, NULL, &DstRect, -Target->m_Degree, &p, SDL_FLIP_NONE);
			/*
			// Draw polygons. 이것도 VIew에 맞춰야함
			b2Fixture* F = m_b2_Body->GetFixtureList();
			SDL_SetRenderDrawColor(Renderer, 0, 255, 0, 100);
			while (F != NULL)
			{
				switch (F->GetType())
				{
					case b2Shape::e_circle:
						break;
					case b2Shape::e_edge:
						break;
					case b2Shape::e_polygon:
					{
					b2PolygonShape* poly = (b2PolygonShape*)F->GetShape();
					b2Vec2 v1, v2, v3;
					v1 = m_b2_Body->GetWorldPoint(poly->m_vertices[0]);
					v2 = m_b2_Body->GetWorldPoint(poly->m_vertices[1]);
					v3 = m_b2_Body->GetWorldPoint(poly->m_vertices[2]);
					SDL_RenderDrawLine(Renderer, v1.x, -v1.y, v2.x, -v2.y);
					SDL_RenderDrawLine(Renderer, v1.x, -v1.y, v3.x, -v3.y);
					SDL_RenderDrawLine(Renderer, v2.x, -v2.y, v3.x, -v3.y);
					}
					break;
					case b2Shape::e_chain:
						break;
					case b2Shape::e_typeCount:
						break;
				}
				F = F->GetNext();
			}
			b2Vec2 v0 = m_b2_Body->GetPosition();
			SDL_SetRenderDrawColor(Renderer, 255, 255, 255, 255);
			SDL_RenderDrawPoint(Renderer, v0.x, -v0.y);
			*/
		}
	}
	
	SDL_RenderPresent(m_Renderer);
}