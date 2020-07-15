#pragma once
#include "../../Engine/Utility/Input.h"
#include "../ImGui/ImGuizmo/ImGuizmo.h"

class Transform;
class D3DSystem;

class Gizmo
{
public:
	Gizmo();
	void Init(D3DSystem* aSystem);
	void Update(Input& input, const ImVec2& pos, const ImVec2& size);
	// Returns true if was manipulated
	bool Manipulate(Transform& aTransform);
	
private:
	ImVec2 myPos;
	ImVec2 mySize;
	ImGuizmo::OPERATION myCurrentOperation;
	ImGuizmo::MODE myCurrentMode;
	int myGizmoIndex;
	D3DSystem* mySystem;
	Input* myInput;
};

