#include "pch.h"
#include "InputHandler.h"
#include <windowsx.h>

using namespace CommonUtilities;

void InputHandler::Update()
{
	myMouseHorizontalDelta = 0;
	myMouseVerticalDelta = 0;
	myMouseWheelDelta = 0;
	for (auto& it : myKeyStates) {
		if (it == KeyState::PRESSED)
		{
			it = KeyState::DOWN;
		}
		if (it == KeyState::RELEASED)
		{
			it = KeyState::UP;
		}
	}

	for (auto& it : myButtonStates) {
		if (it == KeyState::PRESSED)
		{
			it = KeyState::DOWN;
		}
		if (it == KeyState::RELEASED)
		{
			it = KeyState::UP;
		}
	}
}

bool InputHandler::UpdateEvents(const UINT aMessage, const WPARAM aWParam, const LPARAM aLParam)
{
	switch (aMessage)
	{
	case WM_KEYDOWN:
		if (myKeyStates[aWParam] != KeyState::DOWN)
		{
			myKeyStates[aWParam] = KeyState::PRESSED;
		}
		break;
	case WM_KEYUP:
		myKeyStates[aWParam] = KeyState::RELEASED;
		break;
	case WM_MOUSEMOVE:
	{
		const int newX = GET_X_LPARAM(aLParam);
		const int newY = GET_Y_LPARAM(aLParam);
		myMouseHorizontalDelta = static_cast<float>(newX - myMousex);
		myMouseVerticalDelta = static_cast<float>(newY - myMousey);
		myMousex = newX;
		myMousey = newY;
	}
	break;
	case WM_MOUSEWHEEL:
		myMouseWheelDelta += GET_WHEEL_DELTA_WPARAM(aWParam);
		break;
	case WM_LBUTTONDOWN:
		myButtonStates[static_cast<int>(Button::MOUSE_LEFT)] = KeyState::PRESSED;
		break;
	case WM_LBUTTONUP:
		myButtonStates[static_cast<int>(Button::MOUSE_LEFT)] = KeyState::RELEASED;
		break;
	case WM_RBUTTONDOWN:
		myButtonStates[static_cast<int>(Button::MOUSE_RIGHT)] = KeyState::PRESSED;
		break;
	case WM_RBUTTONUP:
		myButtonStates[static_cast<int>(Button::MOUSE_RIGHT)] = KeyState::RELEASED;
		break;
	case WM_MBUTTONDOWN:
		myButtonStates[static_cast<int>(Button::MOUSE_MIDDLE)] = KeyState::PRESSED;
		break;
	case WM_MBUTTONUP:
		myButtonStates[static_cast<int>(Button::MOUSE_MIDDLE)] = KeyState::RELEASED;
		break;
	default:
		return false;
	}
	return true;
}

void InputHandler::Init()
{
	for(auto& it : myKeyStates)
	{
		it = KeyState::UP;
	}
	for (auto& it : myButtonStates)
	{
		it = KeyState::UP;
	}
	myMousex = 0;
	myMousey = 0;
	myMouseHorizontalDelta = 0;
	myMouseVerticalDelta = 0;
	myMouseWheelDelta = 0;
}

bool InputHandler::GetKey(const short aKey)
{
	return myKeyStates[aKey] == KeyState::DOWN || myKeyStates[aKey] == KeyState::PRESSED;
}

bool InputHandler::GetKeyPressed(const short aKey)
{
	return myKeyStates[aKey] == KeyState::PRESSED;
}

bool InputHandler::GetKeyReleased(const short aKey)
{
	return myKeyStates[aKey] == KeyState::RELEASED;
}

bool InputHandler::GetButton(Button aButton)
{
	return myButtonStates[static_cast<int>(aButton)] == KeyState::DOWN;
}

bool InputHandler::GetButtonPressed(Button aButton)
{
	return myButtonStates[static_cast<int>(aButton)] == KeyState::PRESSED;
}

bool InputHandler::GetButtonReleased(Button aButton)
{
	return myButtonStates[static_cast<int>(aButton)] == KeyState::RELEASED;
}

float InputHandler::GetMouseHorizontalDelta()
{
	return myMouseHorizontalDelta;
}

float InputHandler::GetMouseVerticalDelta()
{
	return myMouseVerticalDelta;
}

float InputHandler::GetMouseWheelDelta()
{
	return myMouseWheelDelta;
}

int InputHandler::GetMouseX()
{
	return myMousex;
}

int InputHandler::GetMouseY()
{
	return myMousey;
}

int InputHandler::GetMouseAbsoluteX()
{
	POINT point;
	GetCursorPos(&point);
	return point.x;
}

int InputHandler::GetMouseAbsoluteY()
{
	POINT point;
	GetCursorPos(&point);
	return point.y;
}

void InputHandler::SetMousePosition(const int aX, const int aY)
{
	SetCursorPos(aX, aY);
}

int InputHandler::GetAnyKey()
{
	for(int i = 0; i < myKeyStates.size(); i++)
	{
		if (myKeyStates[i] == KeyState::PRESSED)
			return i;
	}
	return 0;
}
