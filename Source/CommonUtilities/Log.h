#pragma once
#include <string>
#include <iostream>
#include <sstream>
#include <unordered_map>
#include <ctime>

namespace Debug
{
	class Logger : public std::ostream
	{
		friend class LoggerBuff;
	public:
		struct Entry
		{
			time_t time;
			std::string message;
		};
		
		// Write a stream buffer that prefixes each line with Plop
		class LoggerBuff : public std::stringbuf
		{
		public:
			LoggerBuff(Logger* anOwner, std::ostream& str);
			virtual ~LoggerBuff();
			virtual int sync();
			void putOutput();

		private:
			std::ostream& output;
			Logger* myOwner;
		};

		enum class LogType
		{
			Log,
			Warning,
			Error
		};
		
		Logger(LogType type, std::ostream& str);
		std::vector<Entry>& Get() const;
		LogType GetType() const;
		
	private:
		LoggerBuff buffer;
		const LogType myType;
		static std::vector<Entry> myLogs;
		static std::vector<Entry> myWarnings;
		static std::vector<Entry> myErrors;
	};

	static Logger Log(Logger::LogType::Log, std::cout);
	static Logger Warning(Logger::LogType::Warning, std::cout);
	static Logger Error(Logger::LogType::Error, std::cout);
}


