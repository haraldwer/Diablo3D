#include "pch.h"
#include "Log.h"

std::vector<Debug::Logger::Entry> Debug::Logger::myLogs;
std::vector<Debug::Logger::Entry> Debug::Logger::myWarnings;
std::vector<Debug::Logger::Entry> Debug::Logger::myErrors;

Debug::Logger::LoggerBuff::LoggerBuff(Logger* anOwner, std::ostream& str): output(str), myOwner(anOwner)
{
}

Debug::Logger::LoggerBuff::~LoggerBuff()
{
	if (pbase() != pptr())
	{
		putOutput();
	}
}

int Debug::Logger::LoggerBuff::sync()
{
	putOutput();
	return 0;
}

void Debug::Logger::LoggerBuff::putOutput()
{
	Entry log;
	log.time = std::time(0);
	switch (myOwner->myType)
	{
	case LogType::Log: log.message = "[Log]: ";
		break;
	case LogType::Warning: log.message = "[Warning]: ";
		break;
	case LogType::Error: log.message = "[Error]: ";
		break;
	}
	const std::string string(str());
	log.message.append(string.substr(0, string.size() - 1));
	switch (myOwner->myType)
	{
	case LogType::Log: myLogs.push_back(log);
		break;
	case LogType::Warning: myWarnings.push_back(log);
		break;
	case LogType::Error: myErrors.push_back(log);
		break;
	}

	output << str();
	str("");
	output.flush();
}

Debug::Logger::Logger(LogType type, std::ostream& str) : myType(type), std::ostream(&buffer), buffer(this, str)
{	
}

std::vector<Debug::Logger::Entry>& Debug::Logger::Get() const
{
	switch(myType)
	{
		case LogType::Log: return myLogs;
		case LogType::Warning: return myWarnings;
		case LogType::Error: return myErrors;
	}
	return myLogs;
}

Debug::Logger::LogType Debug::Logger::GetType() const
{
	return myType;
}
