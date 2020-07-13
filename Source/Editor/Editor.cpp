#include "Editor.h"
#include "imgui.h"
#include "../D3DX11/D3DSystem.h"
#include "../Engine/Engine.h"
#include "../CommonUtilities/Log.h"

Editor::Editor(D3DSystem& system) : myIsPlaying(false), myEngine(nullptr), mySystem(system), myInspector(), myHierarchy(myInspector)
{
}

void Editor::Init()
{
	myWindows.push_back({ false, "Services",	[&]() { Services(); } });
	myWindows.push_back({ false, "Input",		[&]() { InputMapper(); } });
	myWindows.push_back({ true, "Resources",	[&]() { myResourceBrowser.Update(myEngine); } });
	myWindows.push_back({ true, "Log",			[&]() { myLog.Update(); } });
	myWindows.push_back({ true, "Viewport",	[&]() { myViewport.Update(mySystem); } });
	myWindows.push_back({ true, "Inspector",	[&]() { myInspector.Update(myGizmo, myEngine); } });
	myWindows.push_back({ true, "Hierarchy",	[&]() { myHierarchy.Update(myEngine); } });
	myGizmo.Init(&mySystem);
	myResourceBrowser.Init();
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

	CameraMovement(mySystem, input, myTimer.GetDeltaTime());
	
	myGizmo.Update(input, myViewport.GetPos(), myViewport.GetSize());
	
	ImGui::BeginMainMenuBar();
	if(ImGui::BeginMenu("View"))
	{
		for(auto& it : myWindows)
		{
			if (ImGui::MenuItem(((it.open ? "- " : "+ ") + it.name).c_str()))
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
		ImGui::Text(service.GetName().c_str());
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
	
	const float smoothing = 20.0f;
	static float movementSpeed = 1.0f;
	const float sensitivity = 0.4f;
	
	if (input.GetButton(CommonUtilities::Button::MOUSE_RIGHT))
	{
		movementSpeed += input.GetWheelDelta() * 0.001f * movementSpeed;
		if (movementSpeed <= 0.05f)
			movementSpeed = 0.05f;
		if (movementSpeed > 50.0f)
			movementSpeed = 50.0f;
		
		float movement[3] = {
			input.Get('D') - input.Get('A'),
			input.Get('Q') - input.Get('E'),
			input.Get('W') - input.Get('S')
		};
		auto rot = system.GetCamera().GetRotation();
		const Vec2F mDelta = input.GetMouseDelta();
		rot.x += mDelta.y * sensitivity;
		rot.y += mDelta.x * sensitivity;
		system.GetCamera().SetRotation(rot.x, rot.y, rot.z);

		CommonUtilities::Vector2<float> m = { movement[0], movement[2] };
		if(m.Length() > 1.0f)
			m.Normalize();

		const float forward = m.y;
		const float fangle = (-rot.x * (1.0f / 180.0f) * 3.141592f);
		
		m.y = cos(fangle) * forward;
		const float length = m.Length();
		const float angle = (-rot.y * (1.0f/180.0f) * 3.141592f) + atan2(m.y, m.x);
		myCameraDesiredPosition.x += cos(angle) * length * delta * movementSpeed;
		myCameraDesiredPosition.z += sin(angle) * length * delta * movementSpeed;
		myCameraDesiredPosition.y += (movement[1] + sin(fangle) * forward) * delta * movementSpeed;
	}

	if (delta * smoothing > 1)
		delta = 1 / smoothing;
	myCameraPosition += (myCameraDesiredPosition - myCameraPosition) * smoothing * delta;
	system.GetCamera().SetPosition(myCameraPosition.x, myCameraPosition.y, myCameraPosition.z);
}

LRESULT Editor::MessageHandler(HWND aHwnd, UINT aUint, WPARAM aWparam, LPARAM aLparam) const
{
	if (myEngine)
		myEngine->MessageHandler(aHwnd, aUint, aWparam, aLparam);
	return DefWindowProc(aHwnd, aUint, aWparam, aLparam);
}
