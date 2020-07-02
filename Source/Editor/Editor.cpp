#include "Editor.h"
#include "imgui.h"
#include "../D3DX11/D3DSystem.h"
#include <iostream>
#include "../EngineLoader/EngineLoader.h"

Editor::Editor(D3DSystem& system) : myEngineLoader(nullptr), myIsPlaying(false), mySystem(system)
{
}

void Editor::Init()
{
	myWindows.push_back({ true, "Viewport", [&]() { mySystem.Frame(); } });
	myWindows.push_back({ true, "Properties", [&]() {  } });
	myWindows.push_back({ true, "Log", [&]()
	{
		auto& logs = Debug::Log.Get();
		static std::vector<std::string> entries;
		for(auto& it : logs)
			entries.push_back(it.message.c_str());
		logs.clear();
		
		Debug::Logger* engineLog = myEngineLoader->GetLog();
		if(engineLog)
		{
			auto& engineLogs = engineLog->Get();
			for (auto& it : engineLogs)
				entries.push_back(it.message.c_str());
			engineLogs.clear();
		}

		std::vector<const char*> charArr;
		charArr.reserve(entries.size());
		for (auto& it : entries)
			charArr.push_back(it.c_str());
		
		static int curr = 0;
		ImGui::ListBox("", &curr, charArr.data(), charArr.size());
	}});
	Debug::Log << "Editor initialized" << std::endl;
}

void Editor::Update()
{
	if(!myEngineLoader)
	{
		myEngineLoader = new EngineLoader(mySystem, "Engine2.dll");
		myEngineLoader->Init();
	}

	ImGui::BeginMainMenuBar();
	if(ImGui::BeginMenu("View"))
	{
		for(auto& it : myWindows)
		{
			if (ImGui::MenuItem(it.name.c_str()))
				it.open = !it.open;
		}
		ImGui::EndMenu();
	}
	if (ImGui::MenuItem(myIsPlaying ? "Stop" : "Start"))
		myIsPlaying = !myIsPlaying;
	if (ImGui::MenuItem("Reload"))
	{
		if (myEngineLoader)
			myEngineLoader->Reload();
	}
	ImGui::EndMainMenuBar();

	if (myEngineLoader)
		myEngineLoader->Update(myIsPlaying);

	for(auto& it : myWindows)
	{
		if(it.open)
		{
			if(ImGui::Begin(it.name.c_str(), &it.open))
			{
				it.function();
			}
			ImGui::End();
		}
	}
}

LRESULT Editor::MessageHandler(HWND aHwnd, UINT aUint, WPARAM aWparam, LPARAM aLparam)
{
	return DefWindowProc(aHwnd, aUint, aWparam, aLparam);
}
