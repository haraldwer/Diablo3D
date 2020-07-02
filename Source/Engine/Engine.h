#pragma once

#include "SceneManager.h"
#include "ECS/CSystemManager.h"

class Engine
{
public:
	void Init();
	void Update();
private:
	SceneManager mySceneManager;
	CSystemManager myComponentManager;
};

