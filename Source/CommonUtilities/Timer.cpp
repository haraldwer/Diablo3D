#include "pch.h"
#include "Timer.h"

using namespace CommonUtilities;

Timer::Timer()
{
	myStartTime = std::chrono::high_resolution_clock::now();
	myPreviousTime = myStartTime;
}

void Timer::Update()
{
	const std::chrono::steady_clock::time_point newTime = std::chrono::high_resolution_clock::now();
	const std::chrono::duration<float> difference = newTime - myPreviousTime;
	myDelta = difference.count();
	myPreviousTime = newTime;
}

float Timer::GetDeltaTime() const
{
	return myDelta;
}

double Timer::GetTotalTime() const
{
	const std::chrono::steady_clock::time_point newTime = std::chrono::high_resolution_clock::now();
	const std::chrono::duration<double> difference = std::chrono::high_resolution_clock::now() - myStartTime;
	return difference.count();
}
