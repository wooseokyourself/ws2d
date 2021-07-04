#include <SDL.h>
#include <box2d.h>
#include <iostream>

#include "Renderer.h"
#include "RigidObject.h"
#include "Level.h"
#include "Camera.h"

using namespace std;

const int LEVEL_WIDTH = 2000, LEVEL_HEIGHT = 1000;
const int SCREEN_WIDTH = 640, SCREEN_HEIGHT = 480;

int main(int argc, char* argv[])
{
    // Renderer
	Renderer* OnlyRenderer = Renderer::GetRenderer("WindowTitle", SCREEN_WIDTH, SCREEN_HEIGHT);

	// Camera
	Camera* View = new Camera(SCREEN_WIDTH, SCREEN_HEIGHT);

    // Level
    Level* BasicLevel = new Level(LEVEL_WIDTH, LEVEL_HEIGHT);
    
    // Objects
	RigidObject* TreeA = new RigidObject("assets/tree_s.png", 50, 100);
	RigidObject* TreeB = new RigidObject("assets/tree_s.png", 50, 100);
	RigidObject* TreeC = new RigidObject("assets/tree_s.png", 50, 100);
	RigidObject* TreeD = new RigidObject("assets/tree_s.png", 50, 100);
    TreeA->SetPosition(10, 100);
    TreeB->SetPosition(200, 100);
    TreeC->SetPosition(350, 100);
    TreeD->SetPosition(500, 100);
    TreeA->SetRotation(0.0f);
    TreeB->SetRotation(90.0f);
    TreeC->SetRotation(180.0f);
    TreeD->SetRotation(270.0f);
	BasicLevel->AddObject(TreeA);
	BasicLevel->AddObject(TreeB);
	BasicLevel->AddObject(TreeC);
	BasicLevel->AddObject(TreeD);
    
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
					View->m_Rect.x += 10;
					break;
				case SDLK_LEFT:
					View->m_Rect.x -= 10;
					break;
				case SDLK_UP:
					View->m_Rect.y -= 10;
					break;
				case SDLK_DOWN:
					View->m_Rect.y += 10;
					break;
				case SDLK_ESCAPE:
					GameLoop = false;
					break;
				default:
					break;
				}
			}
		}
        
        BasicLevel->Update();
		OnlyRenderer->Render(BasicLevel, View);
	}
	OnlyRenderer->DestroyRenderer();
	return EXIT_SUCCESS;
}
