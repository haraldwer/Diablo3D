#pragma once

#define _EDITOR
#define WIN32_LEAN_AND_MEAN
#include "Windows.h"
#include <string>
#include <functional>
#include <vector>

#include "Inspector/Gizmo.h"
#include "Inspector/Inspector.h"
#include "Viewport.h"
#include "Hierarchy.h"
#include "Resources/ResourceBrowser.h"
#include "Log.h"
#include "../CommonUtilities/Timer.h"

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
	void Services();
	void InputMapper();
	void CameraMovement(D3DSystem& system, Input& input, float delta);

	Vec3F myCameraPosition;
	Vec3F myCameraDesiredPosition;
	
	CommonUtilities::Timer myTimer;
	bool myIsPlaying;
	Engine* myEngine;
	std::vector<Window> myWindows;
	D3DSystem& mySystem;
	Gizmo myGizmo;
	Inspector myInspector;
	Hierarchy myHierarchy;
	Viewport myViewport;
	ResourceBrowser myResourceBrowser;
	Log myLog;
};
