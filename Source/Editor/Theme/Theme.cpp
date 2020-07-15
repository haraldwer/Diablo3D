#include "Theme.h"
#include "EditorColorPicker.h"

Theme::Theme():
	bg("ThemeBG", { 200.0f / 255.0f, 200.0f / 255.0f, 200.0f / 255.0f, 1 }),
	text("ThemeText", { 0, 0, 0, 1 }),
	base("ThemeBase", { 1, 1, 1, 1 }),
	high("ThemeHigh", 1.0f),
	mid("ThemeMid", 0.75f),
	low("ThemeLow", 1.0f),
	offset("ThemeOffset", 1.0f)
{
}

void Theme::Init()
{
	Load();
}

void Theme::Update()
{
	EditorColorPicker::Select();
	if (ImGui::Button("Reset", ImVec2(-1, 0)))
	{
		Default();
	}
	else
	{
		EditorColorPicker::Apply();
	}
	Save();
}

void Theme::Save()
{
	bg.Set({ EditorColorPicker::bg.x, EditorColorPicker::bg.y, EditorColorPicker::bg.z, EditorColorPicker::bg.w });
	text.Set({ EditorColorPicker::text.x, EditorColorPicker::text.y, EditorColorPicker::text.z, EditorColorPicker::text.w });
	base.Set({ EditorColorPicker::base.x, EditorColorPicker::base.y, EditorColorPicker::base.z, EditorColorPicker::base.w });
	high.Set(EditorColorPicker::high_val);
	mid.Set(EditorColorPicker::mid_val);
	low.Set(EditorColorPicker::low_val);
	offset.Set(EditorColorPicker::window_offset);
}

void Theme::Load()
{
	EditorColorPicker::bg = ImVec4(bg.Get().x, bg.Get().y, bg.Get().z, bg.Get().w);
	EditorColorPicker::text = ImVec4(text.Get().x, text.Get().y, text.Get().z, text.Get().w);
	EditorColorPicker::base = ImVec4(base.Get().x, base.Get().y, base.Get().z, base.Get().w);
	EditorColorPicker::high_val = high.Get();
	EditorColorPicker::mid_val = mid.Get();
	EditorColorPicker::low_val = low.Get();
	EditorColorPicker::window_offset = offset.Get();
	EditorColorPicker::Apply();
}

void Theme::Default()
{
	EditorColorPicker::base = ImVec4(1, 1, 1, 0);
	float val = 200.0f / 255.0f;
	EditorColorPicker::bg = ImVec4(val, val, val, 1);
	EditorColorPicker::text = ImVec4(0, 0, 0, 0);
	EditorColorPicker::high_val = 1.0f;
	EditorColorPicker::mid_val = 0.75f;
	EditorColorPicker::low_val = 1.0f;
	EditorColorPicker::window_offset = 1.0f;
	EditorColorPicker::Apply();
}
