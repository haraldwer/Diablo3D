// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include "Engine.h"
#include "../D3DX11/D3DSystem.h"
#include "Log.h"

Engine* engine = nullptr;

extern "C"
{
    __declspec(dllexport) void Create() {
        engine = new Engine();
    }

    __declspec(dllexport) void Init()
    {
        if (engine)
            engine->Init();
    }

    __declspec(dllexport) void Update()
    {
        if (engine)
            engine->Update();
    }

    __declspec(dllexport) void SetRenderer(void* system) {
        if (system)
            D3DSystem::SetInstance(reinterpret_cast<D3DSystem*>(system));
    }

	__declspec(dllexport) void* GetLog()
    {
        return &Debug::Log;
    }

	__declspec(dllexport) void Destroy() {
		if(engine)
		{
		    delete(engine);
		    engine = nullptr;
		}
	}
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}