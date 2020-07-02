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
