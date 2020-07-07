#include "Editor.h"
#include "imgui.h"
#include "../D3DX11/D3DSystem.h"
#include "../Engine/Engine.h"
#include "../CommonUtilities/Log.h"

Editor::Editor(D3DSystem& system) : myIsPlaying(false), myEngine(nullptr), mySystem(system)
{
}

void Editor::Init()
{
	myWindows.push_back({ false, "Services",	[&]() { Services(); } });
	myWindows.push_back({ false, "Resources",	[&]() {  } });
	myWindows.push_back({ false, "Input",	[&]() { Input(); } });
	myWindows.push_back({ true, "Log",			[&]() { Log(); } });
	myWindows.push_back({ true, "Viewport",	[&]() { myViewport.Update(mySystem); } });
	myWindows.push_back({ true, "Inspector",	[&]() { myInspector.Update(myGizmo, myEngine); } });
	Debug::Log << "Editor initialized" << std::endl;
	myGizmo.Init(&mySystem);
}

void Editor::Update()
{
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

	CameraMovement(mySystem, input, 1);
	
	myGizmo.Update(input);
	
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
		Reload();
	ImGui::EndMainMenuBar();

	for(auto& it : myWindows)
	{
		if(it.open)
		{
			if(ImGui::Begin(it.name.c_str(), &it.open))
				it.function();
			ImGui::End();
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

void Editor::CameraMovement(D3DSystem& system, Input& input, float delta)
{
	if (!myEngine)
		return;
	if (!input.GetButton(CommonUtilities::Button::MOUSE_MIDDLE))
		return;
	float movement[3] = {
		input.Get('D') - input.Get('A'),
		input.Get(VK_SHIFT) - input.Get(VK_CONTROL),
		input.Get('W') - input.Get('S')
	};
	auto pos = system.GetCamera().GetPosition();
	pos.x += movement[0] * delta;
	pos.y += movement[1] * delta;
	pos.z += movement[2] * delta;
	system.GetCamera().SetPosition(pos.x, pos.y, pos.z);
	auto rot = system.GetCamera().GetRotation();
	Vec2F mDelta = input.GetMouseDelta();
	rot.x += mDelta.y * 0.5f;
	rot.y += mDelta.x * 0.5f;
	system.GetCamera().SetRotation(rot.x, rot.y, rot.z);
}

LRESULT Editor::MessageHandler(HWND aHwnd, UINT aUint, WPARAM aWparam, LPARAM aLparam) const
{
	if (myEngine)
		myEngine->MessageHandler(aHwnd, aUint, aWparam, aLparam);
	return DefWindowProc(aHwnd, aUint, aWparam, aLparam);
}
