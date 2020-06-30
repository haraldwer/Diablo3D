#pragma once
#include <string>
#include <fstream>

#define DL_ASSERT(text) DL_Debug::Debug::GetInstance()->AssertMessage(__FILE__,__LINE__,__FUNCTION__, text);
#define DL_DEBUG( ... ) DL_Debug::Debug::GetInstance()->DebugMessage(__LINE__,__FUNCTION__,__VA_ARGS__);
#define DL_PRINT(text) DL_Debug::Debug::GetInstance()->PrintMessage(text);
#define DL_DEBUG_VS( s )\
{\
	std::wostringstream os_;\
	os_ << s /* << " (at line " << __LINE__ << " in file " << __FILE__ << " Time: " << __TIMESTAMP__ << ")" << */ << "\n";\
	OutputDebugStringW( os_.str().c_str() );  \
}

namespace DL_Debug
{
	class Debug
	{
	public:
		static void Create(const std::string& aFile = "DebugLogger.txt");
		static void Destroy();
		static Debug* GetInstance();
		void AssertMessage(const char* aFileName, int aLine, const char* aFunctionName, const char* aString);
		void PrintMessage(const char* aString);
		void DebugMessage(const int aLine, const char* aFileName, const char* aFormattedString, ...);
	private:
		Debug();
		~Debug();
		static Debug* ourInstance;
		std::ofstream myDebugFile;
	};
}
