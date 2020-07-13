#pragma once
#include "../ImGui/imgui.h"
#include "../CommonUtilities/Log.h"

class Log
{
public:
	void Update();
private:
	void AddLogs(Debug::Logger& logger, bool& ScrollToBottom, ImGuiTextBuffer& Buf, ImVector<int>& LineOffsets);
};

