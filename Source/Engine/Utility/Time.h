#pragma once
#include "../CommonUtilities/Timer.h"

class Time
{
public:
	Time();
	~Time();
	static float Delta();
	static float UnscaledDelta();
	static float Timescale();
	static double Total();
	static void SetTimescale(const float aScale);

	void Update();
	
private:
	CommonUtilities::Timer myTimer;
	float myTimescale;
	float myDelta;

	static Time* instance;
};

