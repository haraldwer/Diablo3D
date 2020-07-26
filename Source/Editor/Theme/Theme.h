#pragma once
#include "../Preferences.h"

class Theme
{
public:
	Theme();
	void Init();
	void Update();
private:
	void Save();
	void Load();
	void Light();
	void Dark();

	Preference<CommonUtilities::Vector4<float>> bg;
	Preference<CommonUtilities::Vector4<float>> text;
	Preference<CommonUtilities::Vector4<float>> base;
	Preference<float> high;
	Preference<float> mid;
	Preference<float> low;
	Preference<float> offset;
};

