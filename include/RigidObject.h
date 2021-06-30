#pragma once

#include <vector>
#include <string>
#include <box2d.h>
#include <SDL.h>
#include <SDL_image.h>
#include "Object.h"

class RigidObject : public Object
{
public:
	RigidObject(SDL_Renderer* Renderer, const std::string PngPath, const int w, const int h);
	~RigidObject();
    void SetPosition(const int x, const int y);
    void SetRotation(const float degree);
	virtual void Update();
    virtual void Draw(SDL_Renderer* Renderer); // for debug, override
    void ApplyPhysics(b2World* b2_World);
    
private: // Box2d
	b2Body* m_b2_Body;
    SDL_Surface* Surface;
};

