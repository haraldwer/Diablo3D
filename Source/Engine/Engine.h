#pragma once

#define WIN32_LEAN_AND_MEAN

#include <Windows.h>

#include "CSystemManager.h"
#include "SceneManager.h"

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

