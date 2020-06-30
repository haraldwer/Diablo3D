#include "Editor.h"
#include "imgui.h"
#include "../D3DX11/D3DSystem.h"

Editor::Editor(): myWindowOpen(true), myViewportOpen(true)
{
}

void Editor::Init()
{
	myEngine.Init();
}

void Editor::Update(D3DSystem& system)
{
	ImGui::BeginMainMenuBar();
	if(ImGui::BeginMenu("View"))
	{
		if (ImGui::MenuItem("Window"))
			myWindowOpen = !myWindowOpen;
		if (ImGui::MenuItem("Viewport"))
			myViewportOpen = !myViewportOpen;
		ImGui::EndMenu();
	}
	ImGui::EndMainMenuBar();
	
	if(myWindowOpen)
	{
		if(ImGui::Begin("Window", &myWindowOpen))
		{
			
		}
		ImGui::End();
	}
	if (myViewportOpen)
	{
		if (ImGui::Begin("Viewport", &myViewportOpen))
		{
			myEngine.Update();
			system.Frame();
		}
		ImGui::End();
	}
}
