#pragma once

#define WIN32_LEAN_AND_MEAN

#include <Windows.h>

#include "SceneManager.h"
#include "Components/CSystemManager.h"

class Engine
{
public:
	void Init();
	void Update();
	LRESULT MessageHandler(HWND, UINT, WPARAM, LPARAM);
private:
	SceneManager mySceneManager;
	CSystemManager myComponentManager;
};

