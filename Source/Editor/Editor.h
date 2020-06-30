#pragma once
#include "../Engine/Engine.h"

class D3DSystem;

class Editor
{
public:
	Editor();
	void Init(); 
	void Update(D3DSystem& system);
private:
	Engine myEngine;

	bool myWindowOpen;
	bool myViewportOpen;
};

