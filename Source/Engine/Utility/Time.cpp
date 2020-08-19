#include "Time.h"
#include "CommonUtilities/Log.h"

Time* Time::instance = nullptr;

Time::Time() : myTimescale(1), myDelta(0)
{
	if(instance)
	{
		Debug::Warning << "Time singelton already exists" << std::endl;
		return;
	}
	instance = this;
}

Time::~Time()
{
	if (instance == this)
		instance = nullptr; 
}

float Time::Delta()
{
	return instance->myDelta * instance->myTimescale;
}

float Time::UnscaledDelta()
{
	return instance->myDelta;
}

float Time::Timescale()
{
	return instance->myTimescale;
}

double Time::Total()
{
	return instance->myTimer.GetTotalTime();
}

void Time::Update()
{
	myDelta = myTimer.GetDeltaTime();
}

void Time::SetTimescale(const float aScale)
{
	instance->myTimescale = aScale;
}
