#include <SDL.h>
#include <box2d.h>
#include <iostream>

#include "Interface.h"
#include "Renderer.h"
#include "RigidObject.h"
#include "Level.h"
#include "Camera.h"

using namespace std;

const int LEVEL_WIDTH = 640, LEVEL_HEIGHT = 480;
const int SCREEN_WIDTH = 640, SCREEN_HEIGHT = 480;

int main(int argc, char* argv[])
{
	// I/O Manager
	Interface* OnlyInterface = Interface::GetInterface();

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
		OnlyInterface->Input();
		if (OnlyInterface->IsHeldDown(SDLK_RIGHT))
			View->m_Rect.x += 1;
		if (OnlyInterface->IsHeldDown(SDLK_LEFT))
			View->m_Rect.x -= 1;
		if (OnlyInterface->IsHeldDown(SDLK_UP))
			View->m_Rect.y -= 1;
		if (OnlyInterface->IsHeldDown(SDLK_DOWN))
			View->m_Rect.y += 1;
		if (OnlyInterface->IsHeldDown(SDLK_ESCAPE))
			GameLoop = false;
		b2Vec2 Pos = OnlyInterface->GetMouseLeftClickedPosition();
		if (Pos.x != -1 && Pos.y != -1)
		{
			View->m_Rect.x = Pos.x - View->m_Rect.w / 2;
			View->m_Rect.y = Pos.y - View->m_Rect.h / 2;
		}

        
        BasicLevel->Update();
		OnlyRenderer->Render(BasicLevel, View);
	}
	OnlyRenderer->Destroy();
	return EXIT_SUCCESS;
}
