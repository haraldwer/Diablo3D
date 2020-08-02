#pragma once
#include <vector>
#include <string>
#include "../ImGui/imgui.h"
#include "../ImGui/imgui_internal.h"

namespace ImGui
{
	bool Combo(const char* label, int* currIndex, std::vector<std::string>& values);
	bool ListBox(const char* label, int* currIndex, std::vector<std::string>& values);
}