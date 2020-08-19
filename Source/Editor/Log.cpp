#include "Log.h"
#include "../ImGui/imgui.h"
#include <algorithm>

void Log::Update()
{
	static ImGuiTextBuffer     Buf;
	static ImGuiTextFilter     Filter;
	static ImVector<int>       LineOffsets;        // Index to lines offset
	static bool                ScrollToBottom;

	std::vector<TmpLog> entries;

	AddLogs(Debug::Error, entries);
	AddLogs(Debug::Warning, entries);
	AddLogs(Debug::Log, entries);
	Debug::Logger::Clear();

	if(!entries.empty())
	{
		std::sort(entries.begin(), entries.end(), [](TmpLog& a, TmpLog& b)
		{
			return a.entry.time < b.entry.time;
		});
		for (auto& it : entries)
		{
			int old_size = Buf.size();
			Buf.append(it.entry.message.c_str());
			Buf.append("\n");
			for (int new_size = Buf.size(); old_size < new_size; old_size++)
				if (Buf[old_size] == '\n')
					LineOffsets.push_back(old_size);
		}
		ScrollToBottom = true;
	}
	
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

void Log::AddLogs(Debug::Logger& logger, std::vector<TmpLog>& entries)
{
	auto& logs = logger.Get();
	for(auto& it : logs)
		entries.push_back({ logger.GetType(), it });
}
