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
#include "Theme/Theme.h"
#include "Preferences.h"
#include "CameraController.h"

namespace Debug {
	class Logger;
}

class D3DSystem;
class Engine;

struct Window
{
	Window(const std::string& aName, bool aOpen, std::function<void()> aFunction) : name(aName), open(aName + "Open", aOpen), function(aFunction) { }
	std::string name;
	Preference<bool> open;
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
	void Shutdown();
	
private:
	void Services();
	void InputMapper();

	Preferences myPreferences;
	CommonUtilities::Timer myTimer;
	bool myIsPlaying;
	Engine* myEngine;
	std::vector<Window*> myWindows;
	D3DSystem& mySystem;
	Gizmo myGizmo;
	Inspector myInspector;
	Hierarchy myHierarchy;
	Viewport myViewport;
	ResourceBrowser myResourceBrowser;
	Log myLog;
	Theme myThemeSelector;
	CameraController myCamera;
};
