#include "RigidObject.h"
#include "3rdparty/cpp-marching-squares-master/MarchingSquares.h"
#include "3rdparty/douglas-peucker/polygon-simplify.hh"
#include "3rdparty/polypartition-master/src/polypartition.h"
#include <list>
#include <algorithm>

#include <iostream>
using namespace std;

const float RAD2DEG = 57.2957795131;

RigidObject::RigidObject(SDL_Renderer* Renderer, b2World* b2_World, const std::string PngPath, const int x, const int y, const int w, const int h, const float degree)
	: Renderer(Renderer), Degree(0.0)
{
	SDL_Surface* _Surface = IMG_Load(PngPath.c_str());
	if (_Surface == NULL)
	{
		fprintf(stderr, "CreateRGBSurface failed: %s\n", SDL_GetError());
		exit(1);
	}

// Surface의 w, h를 입력된 w, h로 리사이즈
	cout << "Original surface w x h: " << _Surface->w << ", " << _Surface->h << endl;
	SDL_Surface* Surface = SDL_CreateRGBSurface(0, w, h, _Surface->format->BitsPerPixel, _Surface->format->Rmask, _Surface->format->Gmask, _Surface->format->Bmask, _Surface->format->Amask);
	SDL_BlitScaled(_Surface, NULL, Surface, NULL);
	cout << "Scaled   surface w x h: " << Surface->w << ", " << Surface->h << endl;
	SDL_FreeSurface(_Surface);
	BBox.w = w;
	BBox.h = h;
	

// Surface를 Triangulate 하여 box2d와 링킹 
	
	// Construct thresholded data.
	unsigned char* data = new unsigned char[w * h];
	int bpp = Surface->format->BytesPerPixel;
	Uint8* p;
	Uint32 pixel;
	SDL_Color rgba;
	int debugCnt = 0;
	for (int _y = 0; _y < h; _y++)
	{
		for (int _x = 0; _x < w; _x++)
		{
			// Get a pixel of surface.
			p = (Uint8*)Surface->pixels + _y * Surface->pitch + _x * bpp;
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
				pixel = 0;       /* shouldn't happen, but avoids warnings */
			}
			SDL_GetRGBA(pixel, Surface->format, &rgba.r, &rgba.g, &rgba.b, &rgba.a);
			if ((int)rgba.a != 0)
			{
				debugCnt++;
				data[_x + (w * _y)] = (unsigned char)255; // (x, y) 가 유효한 픽셀이라면
				// cout << "o";
			}
			else
			{
				data[_x + (w * _y)] = (unsigned char)0;
				// cout << "x";
			}
		}
		// cout << endl;
	}

	// Marching-sqaure
	MarchingSquares::Result ms_Result = MarchingSquares::FindPerimeter(w, h, data);
	cout << "ms start: " << "(" << ms_Result.initialX << "," << ms_Result.initialY << "), ms_Result.directions: " << ms_Result.directions.size() << endl;

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
	// if (b2_PolyCorners.size() < 3) 만약 삼각형형성이 안되면 버리는거
	// 	continue;

	TPPLPoly tpp_Poly;
	tpp_Poly.Init((long)b2_PolyCorners.size());

	for (unsigned int i = 0; i < b2_PolyCorners.size(); i++)
		tpp_Poly[(int)b2_PolyCorners.size() - i - 1] = { b2_PolyCorners[i].x, b2_PolyCorners[i].y };

	if (tpp_Poly.GetOrientation() == TPPL_CW)
		tpp_Poly.SetHole(true);

	// 여기쯤에서 Removes Hole 진행해야하는듯 

	// Poly-partition.
	TPPLPartition tpp_Part;
	std::list<TPPLPoly> tpp_Results;
	tpp_Part.Triangulate_EC(&tpp_Poly, &tpp_Results);

	// Pre-process for box2d.
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



	// Define the dynamic body. We set its position and call the body factory.
	b2BodyDef b2_BodyDef;
	b2_BodyDef.type = b2_dynamicBody;
	b2_BodyDef.angle = degree / RAD2DEG;
	b2_BodyDef.position.Set(x, -y);
	b2_Body = b2_World->CreateBody(&b2_BodyDef);

	// fixtures 는 body로부터 transform을 상속받는다.
	for (b2PolygonShape p : b2_Triangles)
	{
		// Define the dynamic body fixture.
		b2FixtureDef b2_FixtureDef;
		b2_FixtureDef.shape = &p;

		// Set the box density to be non-zero, so it will be dynamic.
		b2_FixtureDef.density = 1.0f;

		// Override the default friction.
		b2_FixtureDef.friction = 0.3f;

		b2_FixtureDef.restitution = 0.1f;

		// Add the shape to the body.
		b2_Body->CreateFixture(&b2_FixtureDef);
	}
	cout << "Triangles number: " << b2_Triangles.size() << endl;


	/*
	b2BodyDef b2_BoxBodyDef;
	b2_BoxBodyDef.type = b2_dynamicBody;
	b2_BoxBodyDef.angle = degree / RAD2DEG;
	b2_BoxBodyDef.position.Set(x, y); // x, y는 b2Body의 정중앙 위치

	b2_Body = b2_World->CreateBody(&b2_BoxBodyDef);

	b2PolygonShape dynamicBox;
	dynamicBox.SetAsBox(w, h);

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &dynamicBox;
	fixtureDef.density = 1;
	fixtureDef.friction = 0.3f;
	fixtureDef.restitution = 0.5f;
	b2_Body->CreateFixture(&fixtureDef);
	*/

	// Make a texture of surface
	Texture = SDL_CreateTextureFromSurface(Renderer, Surface);
	if (Texture == NULL)
	{
		fprintf(stderr, "CreateTextureFromSurface failed: %s\n", SDL_GetError());
		exit(1);
	}
	SDL_FreeSurface(Surface);
}

RigidObject::~RigidObject()
{
	SDL_DestroyTexture(Texture);
}

void RigidObject::Update()
{
	b2Vec2 b2_Position = b2_Body->GetPosition();
	Degree = b2_Body->GetAngle() * RAD2DEG; // radian to degree
	BBox.x = b2_Position.x;
	BBox.y = -b2_Position.y;
}

void RigidObject::Draw()
{
	SDL_Point p; p.x = 0; p.y = 0;
	SDL_RenderCopyEx(Renderer, Texture, NULL, &BBox, -Degree, &p, SDL_FLIP_NONE);

	// Draw polygons
	b2Fixture* F = b2_Body->GetFixtureList();
	SDL_SetRenderDrawColor(Renderer, 0, 255, 0, 100);
	while (F != NULL)
	{
		switch (F->GetType())
		{
		case b2Shape::e_polygon:
		{
			b2PolygonShape* poly = (b2PolygonShape*)F->GetShape();
			b2Vec2 v1, v2, v3;
			v1 = b2_Body->GetWorldPoint(poly->m_vertices[0]);
			v2 = b2_Body->GetWorldPoint(poly->m_vertices[1]);
			v3 = b2_Body->GetWorldPoint(poly->m_vertices[2]);
			SDL_RenderDrawLine(Renderer, v1.x, -v1.y, v2.x, -v2.y);
			SDL_RenderDrawLine(Renderer, v1.x, -v1.y, v3.x, -v3.y);
			SDL_RenderDrawLine(Renderer, v2.x, -v2.y, v3.x, -v3.y);
		}
		break;
		}
		F = F->GetNext();
	}
	b2Vec2 v0 = b2_Body->GetPosition();
	SDL_SetRenderDrawColor(Renderer, 255, 255, 255, 255);
	SDL_RenderDrawPoint(Renderer, v0.x, -v0.y);
}