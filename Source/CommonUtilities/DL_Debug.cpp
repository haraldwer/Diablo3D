#include "pch.h"
#include "DL_Debug.h"
#include "DL_assert.h"
#include "Macros.h"
#include "DL_StackWalker.h"
#include <cstdarg>

namespace DL_Debug
{
	Debug* Debug::ourInstance;
	
	void Debug::Create(const std::string& aFile)
	{
		assert(ourInstance == nullptr && "Debugobject already created");
		ourInstance = new Debug();
		ourInstance->myDebugFile.open(aFile);
	}
	void Debug::Destroy()
	{
		assert(ourInstance != nullptr && "No DL_DEBUG created!");
		ourInstance->myDebugFile.close();
		SAFE_DELETE(ourInstance);
	}
	Debug* Debug::GetInstance()
	{
		assert(ourInstance != nullptr && "DL_DEBUG NOT CREATED!");
		return(ourInstance);
	}

	void Debug::AssertMessage(const char* aFileName, int aLine, const char* aFunctionName, const char* aString)
	{
		myDebugFile << "Function: " << aFunctionName << ", Line: " << aLine << ", File: " << aFileName <<  std::endl;
		StackWalker walker;
		walker.ShowCallstack();
		assert(false);
	}

	void Debug::PrintMessage(const char* aString)
	{
		myDebugFile << aString << std::endl;
	}

	void Debug::DebugMessage(const int aLine, const char* aFileName, const char* aFormattedString, ...)
	{
		va_list args;
		va_start(args, aFormattedString);

		while (*aFormattedString != '\0') {
			if (*aFormattedString == 'd') {
				int i = va_arg(args, int);
				myDebugFile << i << '\n';
			}
			else if (*aFormattedString == 'c') {
				// note automatic conversion to integral type
				int c = va_arg(args, int);
				myDebugFile << static_cast<char>(c) << '\n';
			}
			else if (*aFormattedString == 'f') {
				double d = va_arg(args, double);
				myDebugFile << d << '\n';
			}
			++aFormattedString;
		}

		va_end(args);
		myDebugFile << " | " << aFileName << "(" << aLine << ")" << std::endl;
	}

	Debug::Debug()
	{
	}

	Debug::~Debug()
	{
	}
}
