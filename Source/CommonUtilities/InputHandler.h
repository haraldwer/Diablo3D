#pragma once
#include <array>
#include <Windows.h>

namespace CommonUtilities
{
	enum class Button {
		MOUSE_RIGHT,
		MOUSE_LEFT,
		MOUSE_MIDDLE,
		COUNT
	};

	enum class KeyState {
		PRESSED,
		DOWN,
		RELEASED,
		UP,
	};

	class InputHandler
	{
	public:
		void Update();
		bool UpdateEvents(UINT aMessage, WPARAM aWParam, LPARAM aLParam);
		void Init();

		bool GetKey(const short aKey);
		bool GetKeyPressed(const short aKey);
		bool GetKeyReleased(const short aKey);

		bool GetButton(Button aButton);
		bool GetButtonPressed(Button aButton);
		bool GetButtonReleased(Button aButton);

		float GetMouseHorizontalDelta();
		float GetMouseVerticalDelta();
		float GetMouseWheelDelta();

		int GetMouseX();
		int GetMouseY();
		int GetMouseAbsoluteX();
		int GetMouseAbsoluteY();
		void SetMousePosition(int aX, int aY);
		int GetAnyKey();

	private:
		std::array<KeyState, 256> myKeyStates;
		std::array<KeyState, static_cast<size_t>(Button::COUNT)> myButtonStates;
		int myMousex;
		int myMousey;
		float myMouseHorizontalDelta;
		float myMouseVerticalDelta;
		float myMouseWheelDelta;
	};
}



