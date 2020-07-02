#pragma once
#include "../D3DX11/D3DSystem.h"
#include <iostream>
#include "../CommonUtilities/Log.h"

typedef void(__stdcall* Func)();
typedef void(__stdcall* DataFunc)(void*);
typedef void*(__stdcall* RetDataFunc)();

class EngineLoader
{
public:
	EngineLoader(D3DSystem& system, const char* dllPath);
	~EngineLoader();
	void Update(bool run);
	void Init();
	void Reload();
	Debug::Logger* GetLog();
private:
	void Create();
	void Destroy();
	template <class T>
	T Load(const char* func);
	const char* myDLLPath;
	D3DSystem* mySystem;
	DataFunc myUpdate = nullptr;
	Func myInit = nullptr;
	HINSTANCE myEngineDLL = NULL;
};

template <class T>
T EngineLoader::Load(const char* func)
{
	T temp = (T)GetProcAddress(myEngineDLL, func);
	if (!temp)
	{
		Debug::Log << "Could not locate the " << func << " function" << std::endl;
	}
	return temp;
}
