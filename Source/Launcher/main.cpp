
#include "../D3DX11/D3DSystem.h"
#include "../Engine/Engine.h"
#include <iostream>
#include "../Engine/Engine.h"

#define USE_CONSOLE_COMMAND

void InitConsole()
{
#pragma warning( push )
#pragma warning( disable : 4996 )
	AllocConsole();
	freopen("CONIN$", "r", stdin);
	freopen("CONOUT$", "w", stdout);
	freopen("CONOUT$", "w", stderr);

	setbuf(stdin, NULL);
	setbuf(stdout, NULL);
	setbuf(stderr, NULL);
#pragma warning( pop )
}

void CloseConsole()
{
#pragma warning( push )
#pragma warning( disable : 4996 )
	fclose(stdin);
	fclose(stdout);
	fclose(stderr);
#pragma warning( pop )
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{
#ifdef USE_CONSOLE_COMMAND
	InitConsole();
#endif

	D3DSystem System;
	Engine engine;
	CreateParams params;
	params.myInitFunction = [&] { engine.Init(); };
	params.myUpdateFunction = [&] {  engine.Update(true); };
	params.myMessageHandlerFunction = [&](HWND hwnd, UINT uint, WPARAM wparam, LPARAM lparam) { engine.MessageHandler(hwnd, uint, wparam, lparam); };
	if (System.Initialize(params))
		System.Run();
	System.Shutdown();

#ifdef USE_CONSOLE_COMMAND
	CloseConsole();
#endif

	return 0;
}
