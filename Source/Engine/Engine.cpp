#include "Engine.h"

void Engine::Init()
{
	myComponentManager.Init();
	mySceneManager.Init();
}

void Engine::Update()
{
	myComponentManager.Update();
}

LRESULT CALLBACK Engine::MessageHandler(HWND hwnd, UINT uint, WPARAM wParam, LPARAM lParam)
{
	return DefWindowProc(hwnd, uint, wParam, lParam);
}
