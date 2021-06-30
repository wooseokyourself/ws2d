#include <SDL.h>
#include <box2d.h>
#include <iostream>

#include "Renderer.h"
#include "RigidObject.h"
#include "Level.h"

using namespace std;

const int LEVEL_WIDTH = 2000, LEVEL_HEIGHT = 1000;
const int SCREEN_WIDTH = 640, SCREEN_HEIGHT = 480;

int main(int argc, char* argv[])
{
    // Renderer
	SDL_Renderer* Renderer = InitRenderer("title", LEVEL_WIDTH, LEVEL_HEIGHT);

    // Level
    Level* BasicLevel = new Level(LEVEL_WIDTH, LEVEL_HEIGHT);
    
    // Objects
	RigidObject* TreeA = new RigidObject(Renderer, "assets/tree_s.png", 50, 100);
	RigidObject* TreeB = new RigidObject(Renderer, "assets/tree_s.png", 50, 100);
	RigidObject* TreeC = new RigidObject(Renderer, "assets/tree_s.png", 50, 100);
	RigidObject* TreeD = new RigidObject(Renderer, "assets/tree_s.png", 50, 100);
    BasicLevel->AddObject(TreeA);
    BasicLevel->AddObject(TreeB);
    BasicLevel->AddObject(TreeC);
    BasicLevel->AddObject(TreeD);
    TreeA->SetPosition(10, 100);
    TreeB->SetPosition(200, 100);
    TreeC->SetPosition(350, 100);
    TreeD->SetPosition(500, 100);
    TreeA->SetRotation(0.0f);
    TreeB->SetRotation(90.0f);
    TreeC->SetRotation(180.0f);
    TreeD->SetRotation(270.0f);

    // Camera
    SDL_Rect View;
    View.x = 0;
    View.y = 0;
    View.w = SCREEN_WIDTH;
    View.h = SCREEN_HEIGHT;
    
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
        
        BasicLevel->Update();
        BasicLevel->Draw(Renderer);

		SDL_SetRenderDrawColor(Renderer, 0, 0, 0, 0);
		SDL_RenderPresent(Renderer);
	}

	return EXIT_SUCCESS;
}
