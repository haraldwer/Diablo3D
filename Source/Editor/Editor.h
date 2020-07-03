#pragma once

#define _EDITOR
#define WIN32_LEAN_AND_MEAN
#include "Windows.h"
#include <string>
#include <functional>
#include <vector>

#include "imgui.h"

namespace Debug {
	class Logger;
}

class D3DSystem;
class Engine;

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
	void Log();
	void AddLogs(Debug::Logger& logger, bool& ScrollToBottom, ImGuiTextBuffer& Buf, ImVector<int>& LineOffsets);
	void Services();
	
	bool myIsPlaying;
	Engine* myEngine;
	std::vector<Window> myWindows;
	D3DSystem& mySystem;
};

