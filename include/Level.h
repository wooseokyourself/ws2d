#pragma once

#include <list>
#include <box2d.h>
#include <SDL.h>

#include "RigidObject.h"
#include "Renderer.h"

class Level
{
public:
    Level(const int w, const int h);
    ~Level();
    void AddObject(Object* obj);
    void AddObject(RigidObject* obj);
    void Update();
    void Draw(SDL_Renderer* Renderer); // 렌더러를 인자로 받아 모든 Objects 렌더링
private:
    std::list<Object*> m_Objects;
    b2World* m_b2_World;
};
