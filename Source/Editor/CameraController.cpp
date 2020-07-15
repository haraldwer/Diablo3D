#include "CameraController.h"
#include "../ImGui/imgui.h"
#include "../Engine/Engine.h"
#include "../D3DX11/D3DSystem.h"
#include "../Engine/Utility/Input.h"

bool CameraController::myMouseHidden = false;

CameraController::CameraController() :
	myCameraTransformLoaded(false),
	myCameraPos("CameraPos", { 1, 1, 1 }),
	myCameraDir("CameraDir", { 0, 0, 0 }),
	myFirstFrame(true)
{
}

void CameraController::Update(Engine* engine, D3DSystem& system, Input& input, float delta)
{
	if (!engine)
		return;

	if (!ImGui::IsWindowHovered() && !myMouseHidden && !myFirstFrame)
		return;

	myFirstFrame = false;
	
	const float smoothing = 20.0f;
	static float movementSpeed = 1.0f;
	const float sensitivity = 0.2f;

	if (!myCameraTransformLoaded)
	{
		myCameraPosition = myCameraPos.Get();
		myCameraDesiredPosition = myCameraPosition;
		system.GetCamera().SetPosition(myCameraPos.Get().x, myCameraPos.Get().y, myCameraPos.Get().z);
		system.GetCamera().SetRotation(myCameraDir.Get().x, myCameraDir.Get().y, myCameraDir.Get().z);
		myCameraTransformLoaded = true;
	}

	if (input.GetButton(CommonUtilities::Button::MOUSE_RIGHT))
	{
		movementSpeed += input.GetWheelDelta() * 0.001f * movementSpeed;
		if (movementSpeed <= 0.05f)
			movementSpeed = 0.05f;
		if (movementSpeed > 50.0f)
			movementSpeed = 50.0f;

		float movement[3] = {
			input.Get('D') - input.Get('A'),
			input.Get('E') - input.Get('Q'),
			input.Get('W') - input.Get('S')
		};
		auto rot = system.GetCamera().GetRotation();
		const auto mDelta = input.GetMouseDelta();
		rot.x += mDelta.y * sensitivity;
		rot.y += mDelta.x * sensitivity;
		if (rot.x > 90)
			rot.x = 90;
		if (rot.x < -90)
			rot.x = -90;
		system.GetCamera().SetRotation(rot.x, rot.y, rot.z);
		myCameraDir.Set({ rot.x, rot.y, rot.z });

		CommonUtilities::Vector2<float> m = { movement[0], movement[2] };
		if (m.Length() > 1.0f)
			m.Normalize();

		const float forward = m.y;
		const float fangle = (-rot.x * (1.0f / 180.0f) * 3.141592f);

		m.y = cos(fangle) * forward;
		const float length = m.Length();
		const float angle = (-rot.y * (1.0f / 180.0f) * 3.141592f) + atan2(m.y, m.x);
		myCameraDesiredPosition.x += cos(angle) * length * delta * movementSpeed;
		myCameraDesiredPosition.z += sin(angle) * length * delta * movementSpeed;
		myCameraDesiredPosition.y += (movement[1] + sin(fangle) * forward) * delta * movementSpeed;
		myCameraPos.Set(myCameraDesiredPosition);
		if (!myMouseHidden)
		{
			ShowCursor(false);
			myMouseHidden = true;
		}
	}
	else
	{
		if (myMouseHidden)
		{
			SetCursorPos(myCursorPos.x, myCursorPos.y);
			ShowCursor(true);
			myMouseHidden = false;
		}
		myCursorPos = input.GetMouseAbsolutePosition();
	}

	if (delta * smoothing > 1)
		delta = 1 / smoothing;
	myCameraPosition += (myCameraDesiredPosition - myCameraPosition) * smoothing * delta;
	system.GetCamera().SetPosition(myCameraPosition.x, myCameraPosition.y, myCameraPosition.z);
}

bool CameraController::GetIsControlling()
{
	return myMouseHidden;
}
