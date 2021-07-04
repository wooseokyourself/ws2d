#pragma once

#include <list>
#include <box2d.h>

#include "RigidObject.h"
#include "Camera.h"

class Renderer;

class Level
{
friend class Renderer;
public:
    Level(const int w, const int h);
    ~Level();
    void AddObject(Object* obj);
    void AddObject(RigidObject* obj);
    void Update();

private:
    std::list<Object*> m_Objects;
    b2World* m_b2_World;
    int w;
    int h; 
};
