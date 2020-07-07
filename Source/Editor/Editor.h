#pragma once

#define _EDITOR
#define WIN32_LEAN_AND_MEAN
#include "Windows.h"
#include <string>
#include <functional>
#include <vector>

#include "Inspector/Gizmo.h"
#include "imgui.h"
#include "Inspector/Inspector.h"
#include "Viewport.h"

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
	LRESULT MessageHandler(HWND aHwnd, UINT aUint, WPARAM aWparam, LPARAM aLparam) const;
	void Reload();
	
private:
	void Log();
	void AddLogs(Debug::Logger& logger, bool& ScrollToBottom, ImGuiTextBuffer& Buf, ImVector<int>& LineOffsets);
	void Services();
	void InputMapper();
	void CameraMovement(D3DSystem& system, Input& input, float delta);
	
	bool myIsPlaying;
	Engine* myEngine;
	std::vector<Window> myWindows;
	D3DSystem& mySystem;
	Gizmo myGizmo;
	Inspector myInspector;
	Viewport myViewport;
	ImVec2 myViewPos;
	ImVec2 myViewSize;
};
