#include "Level.h"

Level::Level(const int w, const int h)
    : w(w), h(h)
{
    b2Vec2 b2_Gravity(0.0f, -9.81f);
    m_b2_World = new b2World(b2_Gravity);

    // cartesian origin
    float ground_x = 0.0f;
    float ground_y = 0.0f;

    // start ground point
    b2Vec2 startpoint;
    startpoint.x = -w;
    startpoint.y = -h;

    // end ground point
    b2Vec2 endpoint;
    endpoint.x = w * 2;
    endpoint.y = -h;

    // LineGround
    b2BodyDef myGroundDef;
    myGroundDef.type = b2_staticBody;
    myGroundDef.position.Set(ground_x, ground_y); // set the starting position x and y cartesian
    myGroundDef.angle = 0;

    b2Body* groundLineBody = m_b2_World->CreateBody(&myGroundDef);

    b2EdgeShape edgeShape;
    edgeShape.SetTwoSided(startpoint, endpoint); // length -> coordinate vector from to vector

    b2FixtureDef edgeFixtureDef;
    edgeFixtureDef.shape = &edgeShape;
    groundLineBody->CreateFixture(&edgeFixtureDef);
}

Level::~Level()
{
    delete m_b2_World;
}

void Level::AddObject(Object* obj)
{
    m_Objects.push_back(obj);
}

void Level::AddObject(RigidObject* obj)
{
    obj->ApplyPhysics(m_b2_World);
    m_Objects.push_back(obj);
}

void Level::Update()
{
    for(Object* obj : m_Objects)
        obj->Update();
    m_b2_World->Step(1.0f / 60.0f, 6.0f, 2.0f);
}