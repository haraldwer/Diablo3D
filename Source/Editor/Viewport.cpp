#include "Viewport.h"
#include "../D3DX11/D3DSystem.h"
#include "ImGuizmo/ImGuizmo.h"

void Viewport::Update(D3DSystem& system)
{
	myPos = ImGui::GetWindowPos();
	mySize = ImGui::GetWindowSize();
	ImGuizmo::SetRect(myPos.x, myPos.y, mySize.x, mySize.y);
	system.SetResolution(mySize.x, mySize.y);
	system.Frame();
}
