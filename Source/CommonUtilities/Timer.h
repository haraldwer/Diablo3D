#pragma once
#include <chrono>

namespace CommonUtilities
{
	class Timer
	{
	public:
		Timer();
		Timer(const Timer& aTimer) = delete;
		Timer& operator=(const Timer& aTimer) = delete;
		void Update();
		float GetDeltaTime() const;
		double GetTotalTime() const;

	private:
		float myDelta = 1;
		std::chrono::steady_clock::time_point myPreviousTime;
		std::chrono::steady_clock::time_point myStartTime;
	};
}
