#include "Viewport.h"
#include "../D3DX11/D3DSystem.h"
#include "ImGuizmo/ImGuizmo.h"

bool Viewport::myIsFocused;

void Viewport::Update(D3DSystem& system)
{
	myPos = ImGui::GetWindowPos();
	mySize = ImGui::GetWindowSize();
	system.SetResolution(mySize.x, mySize.y);
	system.Frame();
	ImGuizmo::BeginFrame();
	ImGuizmo::SetOrthographic(false);
	ImGuizmo::SetDrawlist();
	ImGuizmo::SetRect(myPos.x, myPos.y, mySize.x, mySize.y);
	myIsFocused = ImGui::IsWindowFocused();
}

ImVec2 Viewport::GetPos() const
{
	return myPos;
}

ImVec2 Viewport::GetSize() const
{
	return mySize;
}

bool Viewport::IsFocused()
{
	return myIsFocused;
}
