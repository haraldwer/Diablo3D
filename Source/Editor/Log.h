#pragma once
#include "../CommonUtilities/Log.h"

class Log
{
public:
	void Update();
private:
	struct TmpLog
	{
		Debug::Logger::LogType type;
		Debug::Logger::Entry entry;
	};
	void AddLogs(Debug::Logger& logger, std::vector<TmpLog>& entries);
};

