#include "Log.h"

void Log::Update()
{
	static ImGuiTextBuffer     Buf;
	static ImGuiTextFilter     Filter;
	static ImVector<int>       LineOffsets;        // Index to lines offset
	static bool                ScrollToBottom;

	AddLogs(Debug::Error, ScrollToBottom, Buf, LineOffsets);
	AddLogs(Debug::Warning, ScrollToBottom, Buf, LineOffsets);
	AddLogs(Debug::Log, ScrollToBottom, Buf, LineOffsets);

	if (ImGui::Button("Clear"))
	{
		Buf.clear();
		LineOffsets.clear();
	}
	ImGui::SameLine();
	bool copy = ImGui::Button("Copy");
	ImGui::SameLine();
	Filter.Draw("Filter", -100.0f);
	ImGui::Separator();
	ImGui::BeginChild("scrolling");
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 1));
	if (copy) ImGui::LogToClipboard();

	if (Filter.IsActive())
	{
		const char* buf_begin = Buf.begin();
		const char* line = buf_begin;
		for (int line_no = 0; line != NULL; line_no++)
		{
			const char* line_end = (line_no < LineOffsets.Size) ? buf_begin + LineOffsets[line_no] : NULL;
			if (Filter.PassFilter(line, line_end))
				ImGui::TextUnformatted(line, line_end);
			line = line_end && line_end[1] ? line_end + 1 : NULL;
		}
	}
	else
	{
		ImGui::TextUnformatted(Buf.begin());
	}

	if (ScrollToBottom)
		ImGui::SetScrollHere(1.0f);
	ScrollToBottom = false;
	ImGui::PopStyleVar();
	ImGui::EndChild();
}

void Log::AddLogs(Debug::Logger& logger, bool& ScrollToBottom, ImGuiTextBuffer& Buf, ImVector<int>& LineOffsets)
{
	auto& logs = logger.Get();
	if (!logs.empty())
	{
		for (auto& it : logs)
		{
			int old_size = Buf.size();
			Buf.append(it.message.c_str());
			Buf.append("\n");
			for (int new_size = Buf.size(); old_size < new_size; old_size++)
				if (Buf[old_size] == '\n')
					LineOffsets.push_back(old_size);
		}
		ScrollToBottom = true;
		logs.clear();
	}
}
