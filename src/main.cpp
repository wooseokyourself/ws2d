#include <SDL.h>
#include <box2d.h>
#include <iostream>

#include "Renderer.h"
#include "RigidObject.h"

using namespace std;

const int WIDTH = 1000, HEIGHT = 480;

int main(int argc, char* argv[])
{
	SDL_Renderer* Renderer = InitRenderer("title", WIDTH, HEIGHT);

	b2Vec2 b2_Gravity(0.0f, -9.81f);
	b2World* b2_World = new b2World(b2_Gravity);

    // cartesian origin
    float ground_x = 0.0f;
    float ground_y = 0.0f;

    // start ground point
    b2Vec2 startpoint;
    startpoint.x = -WIDTH;
    startpoint.y = -HEIGHT;

    // end ground point
    b2Vec2 endpoint;
    endpoint.x = WIDTH * 2;
    endpoint.y = -HEIGHT;

    // LineGround
    b2BodyDef myGroundDef;
    myGroundDef.type = b2_staticBody;
    myGroundDef.position.Set(ground_x, ground_y); // set the starting position x and y cartesian
    myGroundDef.angle = 0;

    b2Body* groundLineBody = b2_World->CreateBody(&myGroundDef);

    b2EdgeShape edgeShape;
    edgeShape.SetTwoSided(startpoint, endpoint); // length -> coordinate vector from to vector

    b2FixtureDef edgeFixtureDef;
    edgeFixtureDef.shape = &edgeShape;
    groundLineBody->CreateFixture(&edgeFixtureDef);




	RigidObject TreeA(Renderer, b2_World, "assets/tree_s.png", 10, 100, 50, 100, 0);
	RigidObject TreeB(Renderer, b2_World, "assets/tree_s.png", 200, 100, 50, 100, 90);
	RigidObject TreeC(Renderer, b2_World, "assets/tree_s.png", 350, 100, 50, 100, 180);
	RigidObject TreeD(Renderer, b2_World, "assets/tree_s.png", 550, 100, 50, 100, 270);

	// Game loop
	bool GameLoop = true;
	while (GameLoop)
	{
		// Interface
		SDL_Event Event;
		while (SDL_PollEvent(&Event))
		{
			if (Event.type == SDL_QUIT)
				break;
			else if (Event.type == SDL_KEYDOWN)
			{
				switch (Event.key.keysym.sym)
				{
				case SDLK_RIGHT:
					cout << "r" << endl;
					break;
				case SDLK_LEFT:
					cout << "l" << endl;
					break;
				case SDLK_UP:
					cout << "u" << endl;
					break;
				case SDLK_DOWN:
					cout << "d" << endl;
					break;
				case SDLK_ESCAPE:
					GameLoop = false;
					break;
				default:
					break;
				}
			}
		}

		// Render
		// SDL_SetRenderDrawColor(Renderer, 0, 0, 0, 255);

		SDL_RenderClear(Renderer);
		
		// Draw ground platform
		SDL_SetRenderDrawColor(Renderer, 255, 0, 0, 0);
		SDL_RenderDrawLine(Renderer, edgeShape.m_vertex1.x, -edgeShape.m_vertex1.y, edgeShape.m_vertex2.x, -edgeShape.m_vertex2.y);

		TreeA.Update();
		TreeA.Draw();
		TreeB.Update();
		TreeB.Draw();
		TreeC.Update();
		TreeC.Draw();
		TreeD.Update();
		TreeD.Draw();

		SDL_SetRenderDrawColor(Renderer, 0, 0, 0, 0);
		SDL_RenderPresent(Renderer);

		b2_World->Step(1.0f / 60.0f, 6.0f, 2.0f); // Update physics
	}

	delete b2_World;

	return EXIT_SUCCESS;
}