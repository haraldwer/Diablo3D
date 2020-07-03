#include "Editor.h"
#include "imgui.h"
#include "../D3DX11/D3DSystem.h"
#include <iostream>
#include "../Engine/Engine.h"
#include "../CommonUtilities/Log.h"

Editor::Editor(D3DSystem& system) : myIsPlaying(false), myEngine(nullptr), mySystem(system)
{
}

void Editor::Init()
{
	myWindows.push_back({ true, "Viewport", [&]()
	{
		auto size = ImGui::GetWindowSize();
		mySystem.SetResolution(size.x, size.y);
		mySystem.Frame();
	} });
	myWindows.push_back({ true, "Inspector", [&]() {  } });
	myWindows.push_back({ true, "Resources", [&]() {  } });
	myWindows.push_back({ true, "Log", [&]() { Log(); } });
	myWindows.push_back({ true, "Services", [&]() { Services(); } });
	Debug::Log << "Editor initialized" << std::endl;
}

void Editor::Update()
{
	if(!myEngine)
	{
		myEngine = new Engine();
		myEngine->Init();
		Debug::Log << "Engine created" << std::endl;
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
		if (myEngine)
		{
			delete(myEngine);
			myEngine = nullptr;
		}
	}
	ImGui::EndMainMenuBar();

	if (myEngine)
		myEngine->Update(myIsPlaying);

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

void Editor::Log()
{
	static ImGuiTextBuffer     Buf;
	static ImGuiTextFilter     Filter;
	static ImVector<int>       LineOffsets;        // Index to lines offset
	static bool                ScrollToBottom;

	AddLogs(Debug::Error, ScrollToBottom, Buf, LineOffsets);
	AddLogs(Debug::Warning, ScrollToBottom, Buf, LineOffsets);
	AddLogs(Debug::Log, ScrollToBottom, Buf, LineOffsets);

	if (ImGui::Button("Clear"))
	{
		Buf.clear();
		LineOffsets.clear();
	}
	ImGui::SameLine();
	bool copy = ImGui::Button("Copy");
	ImGui::SameLine();
	Filter.Draw("Filter", -100.0f);
	ImGui::Separator();
	ImGui::BeginChild("scrolling");
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 1));
	if (copy) ImGui::LogToClipboard();

	if (Filter.IsActive())
	{
		const char* buf_begin = Buf.begin();
		const char* line = buf_begin;
		for (int line_no = 0; line != NULL; line_no++)
		{
			const char* line_end = (line_no < LineOffsets.Size) ? buf_begin + LineOffsets[line_no] : NULL;
			if (Filter.PassFilter(line, line_end))
				ImGui::TextUnformatted(line, line_end);
			line = line_end && line_end[1] ? line_end + 1 : NULL;
		}
	}
	else
	{
		ImGui::TextUnformatted(Buf.begin());
	}

	if (ScrollToBottom)
		ImGui::SetScrollHere(1.0f);
	ScrollToBottom = false;
	ImGui::PopStyleVar();
	ImGui::EndChild();
}

void Editor::AddLogs(Debug::Logger& logger, bool& ScrollToBottom, ImGuiTextBuffer& Buf, ImVector<int>& LineOffsets)
{
	auto& logs = logger.Get();
	if (!logs.empty())
	{
		for (auto& it : logs)
		{
			int old_size = Buf.size();
			Buf.append(it.message.c_str());
			Buf.append("\n");
			for (int new_size = Buf.size(); old_size < new_size; old_size++)
				if (Buf[old_size] == '\n')
					LineOffsets.push_back(old_size);
		}
		ScrollToBottom = true;
		logs.clear();
	}
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
		{
			service.Editor();
		}
	}
}

LRESULT Editor::MessageHandler(HWND aHwnd, UINT aUint, WPARAM aWparam, LPARAM aLparam)
{
	return DefWindowProc(aHwnd, aUint, aWparam, aLparam);
}
