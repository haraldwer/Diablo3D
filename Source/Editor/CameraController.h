#pragma once
#include "../CommonUtilities/Vector2.h"
#include "../CommonUtilities/Vector3.h"
#include "Preferences.h"
class Input;
class D3DSystem;
class Engine;

class CameraController
{
public:
	CameraController();
	void Update(Engine* engine, D3DSystem& system, Input& input, float delta);
	static bool GetIsControlling();
private:
	bool myCameraTransformLoaded;
	CommonUtilities::Vector3<float> myCameraPosition;
	CommonUtilities::Vector3<float> myCameraDesiredPosition;
	CommonUtilities::Vector2<int> myCursorPos;
	static bool myMouseHidden;
	bool myFirstFrame;
	Preference<CommonUtilities::Vector3<float>> myCameraPos;
	Preference<CommonUtilities::Vector3<float>> myCameraDir;
};

