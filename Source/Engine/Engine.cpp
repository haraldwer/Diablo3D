#include "Engine.h"

Engine::~Engine()
{
	myPhysicsManager.Shutdown();
}

void Engine::Init()
{
	myResourceManager.LoadResources("content");
	myPhysicsManager.Init();
	myComponentManager.Init();
	mySceneManager.Init();
	mySceneManager.LoadScene(3);
}

void Engine::Update(bool run)
{
	myComponentManager.Update();
}

void Engine::MessageHandler(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
{
	switch(message)
	{
		
	}
}
