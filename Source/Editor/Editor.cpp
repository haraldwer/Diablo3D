#include "Editor.h"
#include "imgui.h"
#include "../D3DX11/D3DSystem.h"
#include "../Engine/Engine.h"
#include "../CommonUtilities/Log.h"

Editor::Editor(D3DSystem& system) :
myIsPlaying(false),
myEngine(nullptr),
mySystem(system),
myHierarchy(myInspector)
{
}

void Editor::Init()
{
	myWindows.push_back(new Window("Services",	false, [&]() { Services(); } ));
	myWindows.push_back(new Window("Input",	false, [&]() { InputMapper(); } ));
	myWindows.push_back(new Window("Theme",	false, [&]() { myThemeSelector.Update(); } ));
	myWindows.push_back(new Window("Resources",true,	[&]() { myResourceBrowser.Update(myEngine); } ));
	myWindows.push_back(new Window("Log",		true,	[&]() { myLog.Update(); } ));
	myWindows.push_back(new Window("Viewport",	true,	[&]()
	{
		if(myEngine)
		{
			Input& input = myEngine->GetServiceLocator().GetService<Input>();
			myCamera.Update(myEngine, mySystem, input, myTimer.GetDeltaTime());
			myGizmo.Update(input, myViewport.GetPos(), myViewport.GetSize());
		}
		myViewport.Update(mySystem);
	} ));
	myWindows.push_back(new Window("Inspector",true,	[&]() { myInspector.Update(myGizmo, myEngine); } ));
	myWindows.push_back(new Window("Hierarchy",true,	[&]() { myHierarchy.Update(myEngine); } ));
	myPreferences.Load();
	myGizmo.Init(&mySystem);
	myThemeSelector.Init();
	Debug::Log << "Editor initialized" << std::endl;
}

void Editor::Update()
{	
	myTimer.Update();
	
	if(!myEngine)
	{
		myEngine = new Engine();
		myEngine->Init();
		Debug::Log << "Engine created" << std::endl;
	}

	if (myEngine)
		myEngine->Update(myIsPlaying);

	Input& input = myEngine->GetServiceLocator().GetService<Input>();
	if(input.GetPressed(VK_F5))
	{
		Reload();
		myIsPlaying = true;
	}
	
	ImGui::BeginMainMenuBar();
	if(ImGui::BeginMenu("View"))
	{
		for(auto& it : myWindows)
		{
			if (ImGui::MenuItem(((it->open.Get() ? "- " : "+ ") + it->name).c_str()))
				it->open.Set(!it->open.Get());
		}
		ImGui::EndMenu();
	}
	if (ImGui::MenuItem(myIsPlaying ? "Stop" : "Start"))
		myIsPlaying = !myIsPlaying;
	if (ImGui::MenuItem("Reload"))
		Reload();
	ImGui::EndMainMenuBar();
	
	for(auto& it : myWindows)
	{
		if(it->open.Get())
		{
			bool open = it->open.Get();
			if(ImGui::Begin(it->name.c_str(), &open))
				it->function();
			ImGui::End();
			it->open.Set(open);
		}
	}

	if (myEngine)
		myEngine->LateUpdate();
}

void Editor::Reload()
{
	if (myEngine)
	{
		myEngine->Shutdown();
		delete(myEngine);
		myEngine = nullptr;
	}
}

void Editor::Shutdown()
{
	if (myEngine)
	{
		myEngine->Shutdown();
		delete(myEngine);
		myEngine = nullptr;
	}
	myPreferences.Save();
	for (auto& it : myWindows)
		delete(it);
	myWindows.clear();
}

void Editor::Services()
{
	if (!myEngine)
		return;
	
	static int curr = 0;
	std::vector<const char*> serviceNames;
	auto& services = ServiceLocator::Instance().GetServices();
	for(auto& it : services)
	{
		auto& service = *(it.second);
		if(ImGui::CollapsingHeader(service.GetName().c_str()))
			service.Editor();
	}
}

void Editor::InputMapper()
{
	if (!myEngine)
		return;

	Input& input = myEngine->GetServiceLocator().GetService<Input>();
	input.Editor();
}

LRESULT Editor::MessageHandler(HWND aHwnd, UINT aUint, WPARAM aWparam, LPARAM aLparam) const
{
	if (myEngine)
		myEngine->MessageHandler(aHwnd, aUint, aWparam, aLparam);
	return DefWindowProc(aHwnd, aUint, aWparam, aLparam);
}
