#include "FrameCounter.h"

void FrameCounter::Initialize()
{
	m_fps = 0;
	m_count = 0;
	m_startTime = timeGetTime();
}

void FrameCounter::Frame()
{
	m_count++;
	const auto time = timeGetTime();
	if(time >= (m_startTime + 1000))
	{
		m_fps = m_count;
		m_count = 0;
		m_startTime = time;
	}
}

int FrameCounter::GetFps() const
{
	return m_fps;
}
