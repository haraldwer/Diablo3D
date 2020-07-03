#pragma once
#include "Scenes/SceneManager.h"
#include "Utility/ServiceLocator.h"
#include "ECS/CSystemManager.h"
#include "EngineResources/ResourceManager.h"

class Engine
{
public:
	void Init();
	void Update(bool run);
private:
	ServiceLocator myServiceLocator;
	SceneManager mySceneManager;
	CSystemManager myComponentManager;
	ResourceManager myResourceManager;
};

