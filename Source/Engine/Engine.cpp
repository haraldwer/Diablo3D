#include "Engine.h"

Engine::~Engine()
{
}

void Engine::Init()
{
	myInput.Init();
	myResourceManager.LoadResources("content");
	myPhysicsManager.Init();
	myComponentManager.Init();
	mySceneManager.Init();
	mySceneManager.LoadScene(3);
}

void Engine::Shutdown()
{
	myPhysicsManager.Shutdown();
}

void Engine::Update(bool run)
{
	myComponentManager.Update();
	myPhysicsManager.Update();
}

void Engine::LateUpdate()
{
	myInput.Update();
}

void Engine::MessageHandler(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
{
	myInput.MessageHandler(message, wparam, lparam);
}

ServiceLocator& Engine::GetServiceLocator()
{
	return myServiceLocator;
}
