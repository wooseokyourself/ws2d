#pragma once

#include <vector>
#include <string>
#include <box2d.h>
#include <SDL.h>
#include <SDL_image.h>
#include "Renderer.h"

class RigidObject
{
public:
	RigidObject(SDL_Renderer* Renderer, b2World* b2_World, const std::string PngPath, const int x, const int y, const int w, const int h, const float degree);
	~RigidObject();
	void Update();
	void Draw();

private:
	SDL_Renderer* Renderer; // reference

private: // Box2d
	std::vector<b2PolygonShape> b2_Triangles; // for debug
	b2Body* b2_Body; // 누끼따지않고 모든객체를 박스로취급

private: // Texture
	SDL_Texture* Texture;
	SDL_Rect BBox;
	float Degree;
};

