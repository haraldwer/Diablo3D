#pragma once
#include "../D3DX11/D3DSystem.h"
#include "../ImGui/imgui.h"

class Input;

class Viewport
{
public:
	void Update(D3DSystem& system);
	ImVec2 GetPos() const;
	ImVec2 GetSize() const;
	static bool IsFocused();
private:
	static bool myIsFocused;
	ImVec2 myPos;
	ImVec2 mySize;
};

