#pragma once
#include "Scenes/SceneManager.h"
#include "Utility/ServiceLocator.h"
#include "ECS/CSystemManager.h"
#include "EngineResources/ResourceManager.h"
#include "Physics/PhysicsManager.h"

#define WIN32_LEAN_AND_MEAN
#include "Windows.h"
#include "Utility/Input.h"
#include "Utility/Time.h"

class Engine
{
public:
	~Engine();
	void Init();
	void Shutdown();
	void Update(bool run);
	void LateUpdate();
	void MessageHandler(HWND, UINT, WPARAM, LPARAM);
	ServiceLocator& GetServiceLocator();
private:
	ServiceLocator myServiceLocator;
	SceneManager mySceneManager;
	CSystemManager myComponentManager;
	ResourceManager myResourceManager;
	PhysicsManager myPhysicsManager;
	Input myInput;
	Time myTime;
};

