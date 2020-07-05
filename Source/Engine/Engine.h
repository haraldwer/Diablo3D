#pragma once
#include "Scenes/SceneManager.h"
#include "Utility/ServiceLocator.h"
#include "ECS/CSystemManager.h"
#include "EngineResources/ResourceManager.h"
#include "Physics/PhysicsManager.h"

#define WIN32_LEAN_AND_MEAN
#include "Windows.h"

class Engine
{
public:
	~Engine();
	void Init();
	void Update(bool run);
	void MessageHandler(HWND, UINT, WPARAM, LPARAM);
private:
	ServiceLocator myServiceLocator;
	SceneManager mySceneManager;
	CSystemManager myComponentManager;
	ResourceManager myResourceManager;
	PhysicsManager myPhysicsManager;
};

