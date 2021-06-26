#pragma once

#include <vector>
#include <string>
#include <b2_body.h>
#include <SDL.h>
#include <SDL_image.h>
#include "Renderer.h"

class RigidObject
{
public:
	RigidObject(SDL_Renderer* Renderer, const std::string PngPath, const int x, const int y, const int w, const int h);
	~RigidObject();
	void Draw(SDL_Renderer* Renderer);

private: // Box2d
	std::vector<b2PolygonShape*> Triangles;

private: // Texture
	SDL_Texture* Texture;
	SDL_Rect BBox;
	int Degree;
};

