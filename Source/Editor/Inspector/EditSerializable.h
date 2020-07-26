#pragma once
#include  "../ImGui/misc/cpp/imgui_stdlib.h"

inline void EditInt(SerializableBase* base)
{
	ImGui::PushID(base);
	auto ptr = reinterpret_cast<Serializable<int>*>(base);
	auto data = ptr->Get();

	switch(base->GetEditorControls())
	{
	case EditorControls::NONE:
		return;
		
	case EditorControls::INPUT_INT:
		if (ImGui::InputInt(base->GetName().c_str(), &data))
			ptr->Set(data);
		break;
		
	case EditorControls::TEXTURE: 
	case EditorControls::MODEL: 
	case EditorControls::RESOURCE:
		// Choose resource by dragdrop or popup
		
	default:
		if (ImGui::DragInt(base->GetName().c_str(), &data))
			ptr->Set(data);
		break;
	}
	
	if (ptr->IsOverride())
	{
		ImGui::SameLine();
		if (ImGui::Button("Apply"))
		{
			ptr->Apply();
		}
	}
	ImGui::PopID();
}

inline void EditFloat(SerializableBase* base)
{
	ImGui::PushID(base);
	auto ptr = reinterpret_cast<Serializable<float>*>(base);
	auto data = ptr->Get();
	if (ImGui::DragFloat(base->GetName().c_str(), &data))
	{
		ptr->Set(data);
	}
	if (ptr->IsOverride())
	{
		ImGui::SameLine();
		if (ImGui::Button("Apply"))
		{
			ptr->Apply();
		}
	}
	ImGui::PopID();
}

inline void EditBool(SerializableBase* base)
{
	ImGui::PushID(base);
	auto ptr = reinterpret_cast<Serializable<bool>*>(base);
	auto data = ptr->Get();
	if (ImGui::Checkbox(base->GetName().c_str(), &data))
	{
		ptr->Set(data);
	}
	if (ptr->IsOverride())
	{
		ImGui::SameLine();
		if (ImGui::Button("Apply"))
		{
			ptr->Apply();
		}
	}
	ImGui::PopID();
}

inline void EditString(SerializableBase* base)
{
	ImGui::PushID(base);
	auto ptr = reinterpret_cast<Serializable<std::string>*>(base);
	auto data = ptr->Get();
	if (ImGui::InputText(base->GetName().c_str(), &data))
	{
		ptr->Set(data);
	}
	if (ptr->IsOverride())
	{
		ImGui::SameLine();
		if (ImGui::Button("Apply"))
		{
			ptr->Apply();
		}
	}
	ImGui::PopID();
}

inline void EditVec2(SerializableBase* base)
{
	ImGui::PushID(base);
	auto ptr = reinterpret_cast<Serializable<Vec2F>*>(base);
	auto data = ptr->Get();
	float val[2] = { data.x, data.y };
	if (ImGui::DragFloat2(base->GetName().c_str(), val))
	{
		data.x = val[0];
		data.y = val[1];
		ptr->Set(data);
	}
	if (ptr->IsOverride())
	{
		ImGui::SameLine();
		if (ImGui::Button("Apply"))
		{
			ptr->Apply();
		}
	}
	ImGui::PopID();
}

inline void EditVec3(SerializableBase* base)
{
	ImGui::PushID(base);
	auto ptr = reinterpret_cast<Serializable<Vec3F>*>(base);
	auto data = ptr->Get();
	float val[3] = { data.x, data.y, data.z };
	if (ImGui::DragFloat3(base->GetName().c_str(), val))
	{
		data.x = val[0];
		data.y = val[1];
		data.z = val[2];
		ptr->Set(data);
	}
	
	if(ptr->IsOverride())
	{
		ImGui::SameLine();
		if(ImGui::Button("Apply"))
		{
			ptr->Apply();
		}
	}
	ImGui::PopID();
}

inline void EditVec4(SerializableBase* base)
{
	ImGui::PushID(base);
	auto ptr = reinterpret_cast<Serializable<CommonUtilities::Vector4<float>>*>(base);
	auto data = ptr->Get();
	float val[4] = { data.x, data.y, data.z, data.w };
	if (ImGui::DragFloat4(base->GetName().c_str(), val))
	{
		data.x = val[0];
		data.y = val[1];
		data.z = val[2];
		data.z = val[3];
		ptr->Set(data);
	}
	if (ptr->IsOverride())
	{
		ImGui::SameLine();
		if (ImGui::Button("Apply"))
		{
			ptr->Apply();
		}
	}
	ImGui::PopID();
}

inline void EditMatrix(SerializableBase* base)
{
	ImGui::PushID(base);
	auto ptr = reinterpret_cast<Serializable<int>*>(base);
	auto data = ptr->Get();
	if (ImGui::DragInt(base->GetName().c_str(), &data))
	{
		ptr->Set(data);
	}
}

