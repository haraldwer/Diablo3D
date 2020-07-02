#include "pch.h"
#include "Log.h"

std::string Debug::Logger::GetString() const
{
	return myStream.str();
}

std::vector<Debug::Logger::Entry>& Debug::Logger::Get()
{
	return myLogs;
}
