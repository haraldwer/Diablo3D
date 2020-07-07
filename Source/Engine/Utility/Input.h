#pragma once
#include "ServiceLocator.h"
#include "../CommonUtilities/InputHandler.h"
#include "../CommonUtilities/Vector2.h"

typedef CommonUtilities::Vector2<float> Vec2F;

class Input : public Service<Input>
{
public:
	Input();
	std::string GetName() override { return "Input"; }
	void Init();
	void Update();
	void Editor() override;
	bool MessageHandler(UINT message, WPARAM wparam, LPARAM lparam);
	bool GetPressed(char aKey);
	bool Get(char aKey);
	bool GetButton(CommonUtilities::Button aButton);
	Vec2F GetMouseDelta();

private:
	CommonUtilities::InputHandler myInputHandler;
};

