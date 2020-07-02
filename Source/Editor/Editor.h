#pragma once

#define WIN32_LEAN_AND_MEAN
#include "Windows.h"
#include <string>
#include <functional>
#include <vector>

class D3DSystem;
class EngineLoader;

struct Window
{
	bool open;
	std::string name;
	std::function<void()> function;
};

class Editor
{
public:
	Editor(D3DSystem& system);
	void Init(); 
	void Update();
	LRESULT MessageHandler(HWND aHwnd, UINT aUint, WPARAM aWparam, LPARAM aLparam);
private:
	bool myIsPlaying;
	EngineLoader* myEngineLoader;
	std::vector<Window> myWindows;
	D3DSystem& mySystem;
};

