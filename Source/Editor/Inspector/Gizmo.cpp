#include "Gizmo.h"

#include "../D3DX11/D3DSystem.h"
#include "../Engine/Entity/Transform.h"

Gizmo::Gizmo(): myCurrentOperation(ImGuizmo::TRANSLATE), myCurrentMode(ImGuizmo::LOCAL), myGizmoIndex(0),
                mySystem(nullptr), myInput(nullptr)
{
}

void Gizmo::Init(D3DSystem* aSystem)
{
	mySystem = aSystem;
}

void Gizmo::Update(Input& input, const ImVec2& aViewPos, const ImVec2& aViewSize)
{
	myClipRecStart = aViewPos;
	myClipRecEnd = aViewPos;
	myClipRecEnd.x += aViewSize.x;
	myClipRecEnd.y += aViewSize.y;

	if (input.GetButton(CommonUtilities::Button::MOUSE_RIGHT))
		return;
	
	if (input.GetPressed('W'))
		myCurrentOperation = ImGuizmo::TRANSLATE;
	else if (input.GetPressed('E'))
		myCurrentOperation = ImGuizmo::ROTATE;
	else if (input.GetPressed('R'))
		myCurrentOperation = ImGuizmo::SCALE;
	myInput = &input;
}

bool Gizmo::Manipulate(Transform& aTransform)
{
	if (!mySystem)
		return false;

	ImGuizmo::SetID(reinterpret_cast<int>(&aTransform));
	
	bool changed = false;
	static float snap[3] = { 1.f, 1.f, 1.f };
	static float bounds[] = { -0.5f, -0.5f, -0.5f, 0.5f, 0.5f, 0.5f };
	static float boundsSnap[] = { 0.1f, 0.1f, 0.1f };
	static bool boundSizing = false;
	const bool useSnap = myInput && myInput->Get('X');
	
	float view[16];
	mySystem->GetView(view);
	float projection[16];
	mySystem->GetProjection(projection);
	
	float objectMatrix[16];
	aTransform.GetMatrix(objectMatrix);

	if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
	{
		float posArr[3];
		float rotArr[3];
		float scaleArr[3];
		ImGuizmo::DecomposeMatrixToComponents(objectMatrix, posArr, rotArr, scaleArr);
		if (ImGui::DragFloat3("Position", posArr, 0.1f)) 
			changed = true;
		if (ImGui::DragFloat3("Rotation", rotArr, 0.1f))
			changed = true;
		if (ImGui::DragFloat3("Scale", scaleArr, 0.1f))
			changed = true;
		ImGuizmo::RecomposeMatrixFromComponents(posArr, rotArr, scaleArr, objectMatrix);
	}
	
	//ImGui::PushClipRect(myClipRecStart, myClipRecEnd, false);
	ImGuizmo::Manipulate(view, projection, myCurrentOperation, myCurrentMode, objectMatrix, NULL, useSnap ? &snap[0] : NULL, boundSizing ? bounds : NULL, snap ? boundsSnap : NULL);
	//ImGui::PopClipRect();
	aTransform.SetMatrix(objectMatrix);
	if (ImGuizmo::IsUsing())
		changed = true;
	return changed;
}