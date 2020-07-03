#include "Engine.h"

void Engine::Init()
{
	myResourceManager.LoadResources("content");
	mySceneManager.Init();
	myComponentManager.Init();
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
