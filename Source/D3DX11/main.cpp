#include "D3DSystem.h"


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{
	CreateParams params;
	D3DSystem System;
	if (System.Initialize(&params))
		System.Run();
	System.Shutdown();
	return 0;
}