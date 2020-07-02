#pragma once
#include <string>
#include <iostream>
#include <sstream>
#include <unordered_map>
#include <ctime>

namespace Debug
{
	class Logger
	{
		struct Entry
		{
			time_t time;
			std::string message;
		};
		
	public:
		template<class T>
		std::ostream & operator << (const T & value)
		{
			static std::stringstream str;
			str << value;
			if(true)
			{
				Entry log;
				log.time = std::time(0);
				log.message = str.str();
				myLogs.push_back(log);
				str.clear();
			}
			std::cout << value;
			return myStream << value;
		}

		std::string GetString() const;
		std::vector<Entry>& Get();
		
	private:
		std::stringstream myStream;
		std::vector<Entry> myLogs;
	};
	
	static Logger Log;
}