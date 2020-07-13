#include "Input.h"

Input::Input(): myInputHandler()
{
}

void Input::Init()
{
	myInputHandler.Init();
}

void Input::Update()
{
	myInputHandler.Update();
}

void Input::Editor()
{
	
}

bool Input::MessageHandler(const UINT message, const WPARAM wparam, const LPARAM lparam)
{
	return myInputHandler.UpdateEvents(message, wparam, lparam);
}

bool Input::GetPressed(const char aKey)
{
	return myInputHandler.GetKeyPressed(aKey);
}

bool Input::Get(const char aKey)
{
	return myInputHandler.GetKey(aKey);
}

bool Input::GetButton(CommonUtilities::Button aButton)
{
	return myInputHandler.GetButton(aButton);
}

Vec2F Input::GetMouseDelta()
{
	return { myInputHandler.GetMouseHorizontalDelta(), myInputHandler.GetMouseVerticalDelta() };
}

float Input::GetWheelDelta()
{
	return myInputHandler.GetMouseWheelDelta();
}
