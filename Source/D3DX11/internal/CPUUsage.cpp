#include "CPUUsage.h"

void CPUUsage::Initialize()
{
	PDH_STATUS status;
	m_canReadCpu = true;

	// Create a query object to poll cpu usage.
	status = PdhOpenQuery(NULL, 0, &m_queryHandle);
	if (status != ERROR_SUCCESS)
		m_canReadCpu = false;

	// Set query object to poll all cpus in the system.
	status = PdhAddCounter(m_queryHandle, TEXT("\\Processor(_Total)\\% processor time"), 0, &m_counterHandle);
	if (status != ERROR_SUCCESS)
		m_canReadCpu = false;

	m_lastSampleTime = GetTickCount();
	m_cpuUsage = 0;
}

void CPUUsage::Shutdown()
{
	if (m_canReadCpu)
		PdhCloseQuery(m_queryHandle);
}

void CPUUsage::Frame()
{
	PDH_FMT_COUNTERVALUE value;
	if (m_canReadCpu)
	{
		if ((m_lastSampleTime + 1000) < GetTickCount())
		{
			m_lastSampleTime = GetTickCount();

			PdhCollectQueryData(m_queryHandle);

			PdhGetFormattedCounterValue(m_counterHandle, PDH_FMT_LONG, NULL, &value);

			m_cpuUsage = value.longValue;
		}
	}
}

int CPUUsage::GetCpuPercentage()
{
	if (m_canReadCpu)
		return static_cast<int>(m_cpuUsage);
	return 0;
}
