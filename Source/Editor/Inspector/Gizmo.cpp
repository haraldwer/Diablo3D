#include "Gizmo.h"

#include "../D3DX11/D3DSystem.h"
#include "../Engine/Entity/Transform.h"

#include "../CommonUtilities/Matrix4x4.h"

Gizmo::Gizmo(): myCurrentOperation(), myCurrentMode(), myGizmoIndex(0), mySystem(nullptr)
{
}

void Gizmo::Init(D3DSystem* aSystem)
{
	mySystem = aSystem;
}

void Gizmo::Update(Input& input)
{
	if (input.GetPressed('W'))
		myCurrentOperation = ImGuizmo::TRANSLATE;
	else if (input.GetPressed('E'))
		myCurrentOperation = ImGuizmo::ROTATE;
	else if (input.GetPressed('R'))
		myCurrentOperation = ImGuizmo::SCALE;
	myInput = &input;
}

inline void GetRotation(float& x, float& y, float& z, const CommonUtilities::Matrix4x4<float>& object)
{
	 x = atan2(-object(2, 3), object(3, 3));
	 const float cosYangle = sqrt(pow(object(1, 1), 2) + pow(object(1, 2), 2));
	 y = atan2(object(1, 3), cosYangle);
	 const float sinXangle = sin(x);
	 const float cosXangle = cos(x);
	 z = atan2(cosXangle * object(2, 1) + sinXangle * object(3, 1), cosXangle * object(2, 2) + sinXangle * object(3, 2));

	//if (object(1, 1) == 1.0f || object(1, 1) == -1.0f)
	//{
	//	Yaw = atan2f(object(1, 3), object(3, 4));
	//	Pitch = 0;
	//	Roll = 0;
	//}
	//else
	//{
	//	Yaw = atan2(-object(2, 1), object(1, 1));
	//	Pitch = asin(object(2, 1));
	//	Roll = atan2(-object(2, 3), object(2, 2));
	//}
}

bool Gizmo::Manipulate(Transform& aTransform)
{
	if (!mySystem)
		return false;
	
	ImGuizmo::SetID(reinterpret_cast<int>(&aTransform));
	
	static float snap[3] = { 1.f, 1.f, 1.f };
	static float bounds[] = { -0.5f, -0.5f, -0.5f, 0.5f, 0.5f, 0.5f };
	static float boundsSnap[] = { 0.1f, 0.1f, 0.1f };
	static bool boundSizing = false;
	const bool useSnap = myInput->Get('X');
	
	float view[16];
	mySystem->GetView(view);
	float projection[16];
	mySystem->GetProjection(projection);

	Vec3F pos = aTransform.GetPosition();
	Vec3F rot = aTransform.GetRotation();
	Vec3F scale = aTransform.GetScale();

	const float rotMul = 360.0f / 3.14f;
	float matrixTranslation[3] = { pos.x, pos.y, pos.z };
	float matrixRotation[3] = { rot.x * rotMul, rot.y * rotMul, rot.z * rotMul};
	float matrixScale[3] = { scale.x, scale.y, scale.z };

	float objectMatrix[16];
	ImGuizmo::RecomposeMatrixFromComponents(matrixTranslation, matrixRotation, matrixScale, objectMatrix);
	ImGuizmo::Manipulate(view, projection, myCurrentOperation, myCurrentMode, objectMatrix, NULL, useSnap ? &snap[0] : NULL, boundSizing ? bounds : NULL, snap ? boundsSnap : NULL);
	ImGuizmo::DecomposeMatrixToComponents(objectMatrix, matrixTranslation, matrixRotation, matrixScale);
	
	aTransform.SetRotation({ matrixRotation[0] / rotMul, matrixRotation[1] / rotMul, matrixRotation[2] / rotMul });	
	aTransform.SetPosition({ matrixTranslation[0], matrixTranslation[1], matrixTranslation[2] });
	aTransform.SetScale({ matrixScale[0], matrixScale[1], matrixScale[2] })
;
	return ImGuizmo::IsUsing();
}