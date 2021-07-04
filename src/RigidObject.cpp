#include "RigidObject.h"
#include "3rdparty/cpp-marching-squares-master/MarchingSquares.h"
#include "3rdparty/douglas-peucker/polygon-simplify.hh"
#include "3rdparty/polypartition-master/src/polypartition.h"
#include <list>
#include <algorithm>

#include <iostream>
using namespace std;

const float RAD2DEG = 57.2957795131;

RigidObject::RigidObject(const std::string PngPath, const int w, const int h)
	: Object()
{
    m_Surface = PngToSurface(PngPath, w, h);
    m_WorldBox.w = w;
    m_WorldBox.h = h;
}

RigidObject::~RigidObject()
{
    // maybe remove object from box2d world..
}

void RigidObject::SetPosition(const int x, const int y)
{
    Object::SetPosition(x, y);
    if (m_b2_Body)
        m_b2_Body->SetTransform(b2Vec2(x, -y), m_Degree);
}

void RigidObject::SetRotation(const float degree)
{
    Object::SetRotation(degree);
    if (m_b2_Body)
        m_b2_Body->SetTransform(b2Vec2(m_WorldBox.x, -m_WorldBox.y), degree);
}


void RigidObject::Update()
{
	const b2Vec2& b2_Position = m_b2_Body->GetPosition();
	m_Degree = m_b2_Body->GetAngle() * RAD2DEG; // radian to degree
	m_WorldBox.x = b2_Position.x;
    m_WorldBox.y = -b2_Position.y;
}

void RigidObject::ApplyPhysics(b2World* b2_World)
{
    // Construct thresholded data.
    const int w = m_Surface->w;
    const int h = m_Surface->h;
    unsigned char* data = new unsigned char[w * h];
    int bpp = m_Surface->format->BytesPerPixel;
    Uint8* p;
    Uint32 pixel;
    SDL_Color rgba;
    for (int _y = 0; _y < h; _y++)
    {
        for (int _x = 0; _x < w; _x++)
        {
            // Get a pixel of surface.
            p = (Uint8*)m_Surface->pixels + _y * m_Surface->pitch + _x * bpp;
            switch (bpp)
            {
            case 1:
                pixel = *p;
                break;

            case 2:
                pixel = *(Uint16*)p;
                break;

            case 3:
                if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
                    pixel = p[0] << 16 | p[1] << 8 | p[2];
                else
                    pixel = p[0] | p[1] << 8 | p[2] << 16;
                break;

            case 4:
                pixel = *(Uint32*)p;
                break;

            default:
                pixel = 0;       // Shouldn't happen, but avoids warnings.
            }
            SDL_GetRGBA(pixel, m_Surface->format, &rgba.r, &rgba.g, &rgba.b, &rgba.a);
            if ((int)rgba.a != 0)
                data[_x + (w * _y)] = (unsigned char)255; // If (x, y) is valid pixels
            else
                data[_x + (w * _y)] = (unsigned char)0;
        }
    }

    // Marching-sqaure.
    MarchingSquares::Result ms_Result = MarchingSquares::FindPerimeter(w, h, data);

    // Convert the result of marching-squares points to b2Vec.
    std::vector<b2Vec2> b2_PolyCorners;
    b2_PolyCorners.push_back(b2Vec2(ms_Result.initialX, ms_Result.initialY));
    for (unsigned int i = 0; i < ms_Result.directions.size(); i++)
    {
        b2Vec2 v;
        v.x = b2_PolyCorners.back().x + ms_Result.directions[i].x;
        v.y = (b2_PolyCorners.back().y - ms_Result.directions[i].y);
        b2_PolyCorners.push_back(v);
    }

    // Douglas-Peucker.
    b2_PolyCorners = DouglasPeucker::simplify(b2_PolyCorners, 2);

    // Pre-process for poly-partition.
    TPPLPoly tpp_Poly;
    tpp_Poly.Init((long)b2_PolyCorners.size());

    for (unsigned int i = 0; i < b2_PolyCorners.size(); i++)
        tpp_Poly[(int)b2_PolyCorners.size() - i - 1] = { b2_PolyCorners[i].x, b2_PolyCorners[i].y };

    if (tpp_Poly.GetOrientation() == TPPL_CW)
        tpp_Poly.SetHole(true);

// Remove Holes here.

    // Poly-partition.
    TPPLPartition tpp_Part;
    std::list<TPPLPoly> tpp_Results;
    tpp_Part.Triangulate_EC(&tpp_Poly, &tpp_Results);

    // Pre-process for box2d.
    std::vector<b2PolygonShape> b2_Triangles;
    std::for_each(tpp_Results.begin(), tpp_Results.end(), [&](TPPLPoly cur)
    {
        b2Vec2 vec[3] =
        {
            {(float)cur[0].x, -(float)cur[0].y},
            {(float)cur[1].x, -(float)cur[1].y},
            {(float)cur[2].x, -(float)cur[2].y}
        };
        if (!(vec[0] == vec[1] || vec[0] == vec[2] || vec[1] == vec[2]))
        {
            b2PolygonShape b2_Shape;
            b2_Shape.Set(vec, 3);
            b2_Triangles.push_back(b2_Shape);
        }
    });

    // Define the dynamic body and add it to the box2d world.
    b2BodyDef b2_BodyDef;
    b2_BodyDef.type = b2_dynamicBody;
    b2_BodyDef.angle = m_Degree / RAD2DEG;
    // b2_BodyDef.position.Set(x, -y);
    b2_BodyDef.position.Set(m_WorldBox.x, -m_WorldBox.y);
    m_b2_Body = b2_World->CreateBody(&b2_BodyDef);
    for (b2PolygonShape p : b2_Triangles)
    {
        b2FixtureDef b2_FixtureDef;
        b2_FixtureDef.shape = &p;
        b2_FixtureDef.density = 1.0f;
        b2_FixtureDef.friction = 0.3f;
        b2_FixtureDef.restitution = 0.1f;
        m_b2_Body->CreateFixture(&b2_FixtureDef);
    }

    m_b2_Body->SetTransform(b2Vec2(m_WorldBox.x, -m_WorldBox.y), m_Degree);
}
