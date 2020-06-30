#pragma once

#pragma comment(lib, "winmm.lib")

#include <windows.h>
#include <mmsystem.h>

class FrameCounter
{
public:
	void Initialize();
	void Frame();
	int GetFps() const;

private:
	int m_fps, m_count;
	unsigned long m_startTime;
};