#include "Inspector.h"
#include "../Viewport.h"
#include "../Hierarchy.h"
#include "../../Engine/ECS/SystemBase.h"
//#include "EditSerializable.h"
#include "../ImGui/misc/cpp/imgui_stdlib.h"
#include "../Engine/Utility/ImGUIUtility.h"

#define SERIALIZABLE_FUNC(type, func) mySerializableFunctions[std::type_index(typeid(type))] = [&](SerializableBase* base, CommandQueue& queue, const std::string& aSystemName) { func(base, queue, aSystemName); };

Inspector::Inspector(): mySelectedID("InspectorSelectedID", -1), mySelectedScene("InspectorSelectedSceneID", -1), myEngine(nullptr)
{
	SERIALIZABLE_FUNC(int								, EditInt);
	SERIALIZABLE_FUNC(float								, EditFloat);
	SERIALIZABLE_FUNC(std::string						, EditString);
	SERIALIZABLE_FUNC(bool								, EditBool);
	SERIALIZABLE_FUNC(Vec2F								, EditVec2);
	SERIALIZABLE_FUNC(Vec3F								, EditVec3);
	SERIALIZABLE_FUNC(CommonUtilities::Vector4<float>	, EditVec4);
}

bool Inspector::GetIsSelected(SceneID aSceneID, EntityID aEntityID)
{
	return (aSceneID == mySelectedScene.Get() && aEntityID == mySelectedID.Get());
}

Entity* Inspector::GetEntity(SceneID sceneID, EntityID entityID) const
{
	if (!myEngine)
		return nullptr;
	if (sceneID == -1)
		return nullptr;
	if (entityID == -1)
		return nullptr;
	ServiceLocator& serviceLocator = myEngine->GetServiceLocator();
	SceneManager& sceneManager = serviceLocator.GetService<SceneManager>();
	Scene* scene = sceneManager.GetScene(sceneID);
	if (!scene)
		return nullptr;
	Entity* entity = scene->GetEntity(entityID);
	if (!entity)
		return nullptr;
	return entity;
}

void Inspector::EditTranform(Transform& aTransform)
{
	Command* c = new Command();
	c->sceneID = mySelectedScene.Get();
	c->entityID = mySelectedID.Get();
	c->data.SetValue<Vec3F>(0, aTransform.GetPosition());
	c->data.SetValue<Vec3F>(1, aTransform.GetRotation());
	c->data.SetValue<Vec3F>(2, aTransform.GetScale());
	c->revert = [&](Command& command)
	{
		Entity* e = GetEntity(command.sceneID, command.entityID);
		if (!e)
			return;
		
		Vec3F tempPos = e->GetTransform().GetPosition();
		Vec3F tempRot = e->GetTransform().GetRotation();
		Vec3F tempScale = e->GetTransform().GetScale();

		e->GetTransform().SetPosition(command.data.GetValue<Vec3F>(0));
		e->GetTransform().SetRotation(command.data.GetValue<Vec3F>(1));
		e->GetTransform().SetScale(command.data.GetValue<Vec3F>(2));

		command.data.SetValue<Vec3F>(0, tempPos);
		command.data.SetValue<Vec3F>(1, tempRot);
		command.data.SetValue<Vec3F>(2, tempScale);
	};
	c->redo = c->revert;
	myCommandQueue.Add(c);
}

void Inspector::EditComponents(Entity* anEntity)
{
	if (!anEntity)
		return;
	
	ServiceLocator& serviceLocator = myEngine->GetServiceLocator();
	auto& sysMan = serviceLocator.GetService<CSystemManager>();
	auto comps = anEntity->GetSystemRefs();
	for(auto& comp : comps)
	{
		auto sys = sysMan.GetSystem(comp);
		if (!sys)
		{
			ImGui::CollapsingHeader("Unable to find system reference");
			continue;
		}
		
		if (ImGui::CollapsingHeader(sys->GetName().c_str(), ImGuiTreeNodeFlags_DefaultOpen))
		{
			ImGui::PushID(sys);
			auto& properties = sys->GetEntityProperties(anEntity->GetID());
			for(auto& it : properties)
			{
				auto type = it.second->GetType();
				if (mySerializableFunctions[type])
					mySerializableFunctions[type](it.second, myCommandQueue, sys->GetName());
			}
			sys->EditEntity(anEntity->GetID());
			ImGui::PopID();
		}
	}

	ImGui::Separator();

	static int curr = 0;
	auto systems = sysMan.GetSystems();
	ImGui::Combo("##SysList", &curr, systems);
	ImGui::SameLine();
	if(ImGui::Button("Add"))
	{
		auto sys = sysMan.GetSystem(systems[curr]);
		if(sys)
		{
			sys->AddEntity(anEntity);
			anEntity->AddSystem(sys->GetTypeIndex());
		}
	}
}

void Inspector::Update(Gizmo& aGizmo, Engine* anEngine)
{
	myEngine = anEngine;
	if (!myEngine)
		return;
	
	Entity* entity = GetEntity(mySelectedScene.Get(), mySelectedID.Get());
	if(!entity)
	{
		mySelectedScene.Set(-1);
		mySelectedID.Set(-1);
		ImGui::Text("No entity selected");
	}
	else
	{
		EditName(entity);
		
		if (aGizmo.Manipulate(entity->GetTransform()))
		{
			if(!myIsManipulating)
			{
				EditTranform(entity->GetTransform());
				myIsManipulating = true;
			}
		}
		else
			if(myIsManipulating)
				myIsManipulating = false;

		EditComponents(entity);
	}
	
	if (ImGui::IsWindowFocused() || Viewport::IsFocused() || Hierarchy::IsFocused())
	{
		Input& input = myEngine->GetServiceLocator().GetService<Input>();
		if (input.Get(VK_CONTROL))
		{
			if (input.GetPressed('Z'))
				myCommandQueue.Revert();
			if (input.GetPressed('Y'))
				myCommandQueue.Redo();
			if (input.GetPressed('S'))
				Save();
			if (input.GetPressed('D'))
				Duplicate();
		}
		if (input.GetPressed(VK_DELETE))
		{
			Delete();
		}
	}
}

void Inspector::EditName(Entity* anEntity)
{
	if (!anEntity)
		return;

	std::string name = anEntity->GetName();
	std::string prefabName = "";
	if(anEntity->GetPrefabID() != -1)
	{
		ServiceLocator& serviceLocator = myEngine->GetServiceLocator();
		PrefabManager& prefabManager = serviceLocator.GetService<PrefabManager>();
		auto prefab = prefabManager.GetPrefab(anEntity->GetPrefabID());
		if (prefab)
			prefabName = prefab->GetName();
		if (name == "")
			name = prefabName;
	}
	
	if (ImGui::InputText("Name", &name))
	{
		anEntity->SetName(name == prefabName ? "" : name);
		auto c = new Command();
		c->entityID = mySelectedID.Get();
		c->sceneID = mySelectedScene.Get();
		c->data.SetValue<std::string>(0, name);
		c->revert = [&](Command& aCommand)
		{
			// Recover
			if (!myEngine)
				return;
			if (aCommand.sceneID == -1)
				return;
			if (aCommand.entityID == -1)
				return;
			ServiceLocator& serviceLocator = myEngine->GetServiceLocator();
			SceneManager& sceneManager = serviceLocator.GetService<SceneManager>();
			Scene* scene = sceneManager.GetScene(aCommand.sceneID);
			if (!scene)
				return;
			if (!scene->ShowEntity(aCommand.entityID))
				return;
			Entity* entity = GetEntity(aCommand.sceneID, aCommand.entityID);
			if (!entity)
				return;
			std::string tempName = entity->GetName();
			entity->SetName(aCommand.data.GetValue<std::string>(0));
			aCommand.data.SetValue<std::string>(0, tempName);
		};
		c->redo = c->revert;
		myCommandQueue.Add(c);
	}
	
	if (ImGui::BeginPopupContextItem("Submenu"))
	{
		if (ImGui::MenuItem("Apply to prefab"))
		{
			// Apply name to prefab!
		}
		ImGui::EndPopup();
	}
}

void Inspector::SetEntity(const SceneID aScene, const EntityID aEntity)
{
	mySelectedID.Set(aEntity);
	mySelectedScene.Set(aScene);
}

void Inspector::Save()
{
	ServiceLocator& serviceLocator = myEngine->GetServiceLocator();
	SceneManager& sceneManager = serviceLocator.GetService<SceneManager>();
	sceneManager.SaveScenes();
	PrefabManager& prefabManager = serviceLocator.GetService<PrefabManager>();
	prefabManager.Save();
}

void Inspector::Duplicate()
{
	if (!myEngine)
		return;
	
	if(mySelectedID.Get() == -1 || mySelectedScene.Get() == -1)
	{
		Debug::Error << "Unable to duplicate entity, no entity selected" << std::endl;
		return;
	}
	ServiceLocator& serviceLocator = myEngine->GetServiceLocator();
	SceneManager& sceneManager = serviceLocator.GetService<SceneManager>();
	Scene* scene = sceneManager.GetScene(mySelectedScene.Get());
	if(!scene)
	{
		Debug::Error << "Unable to find scene with ID " << mySelectedScene.Get() << std::endl;
		return;
	}
	Entity* entity = GetEntity(mySelectedScene.Get(), mySelectedID.Get());
	if (!entity)
	{
		Debug::Error << "Unable to find entity with ID " << mySelectedID.Get() << std::endl;
		return;
	}
	
	auto duplicate = scene->CreateEntity(entity->GetPrefabID());
	if(!duplicate)
	{
		Debug::Error << "Unable to create duplicate of entity with ID " << mySelectedID.Get() << std::endl;
		return;
	}

	// Copy transform
	duplicate->GetTransform().SetMatrix(entity->GetTransform().GetMatrix());
	
	// Also copy component overrides!
	auto sys = entity->GetSystemRefs();
	auto& cSysMan = serviceLocator.GetService<CSystemManager>();
	for(auto& it : sys)
	{
		auto sys = cSysMan.GetSystem(it);
		if (!sys)
			continue;
		auto& src = sys->GetEntityProperties(entity->GetID());
		auto& dest = sys->GetEntityProperties(duplicate->GetID());
		for(auto& it : src)
		{
			auto find = dest.find(it.first);
			if(find != dest.end())
				find->second->Copy(it.second);
		}
	}
	
	SetEntity(duplicate->GetSceneID(), duplicate->GetID());

	// And add to command queue
	HideShowCommand(false);
	Debug::Log << "Entity duplicated" << std::endl;
}

void Inspector::Delete()
{
	HideShowCommand(true);
}

void Inspector::HideShowCommand(bool aPerform)
{
	auto c = new Command();
	c->entityID = mySelectedID.Get();
	c->sceneID = mySelectedScene.Get();
	
	std::function<void(Command& aCommand)> recover = [&](Command& aCommand)
	{
		// Recover
		if (!myEngine)
			return;
		if (aCommand.sceneID == -1)
			return;
		if (aCommand.entityID == -1)
			return;
		ServiceLocator& serviceLocator = myEngine->GetServiceLocator();
		SceneManager& sceneManager = serviceLocator.GetService<SceneManager>();
		Scene* scene = sceneManager.GetScene(aCommand.sceneID);
		if (!scene)
			return;
		if (!scene->ShowEntity(aCommand.entityID))
			return;
		Entity* entity = GetEntity(aCommand.sceneID, aCommand.entityID);
		if (!entity)
			return;
		mySelectedScene.Set(aCommand.sceneID);
		mySelectedID.Set(aCommand.entityID);
		//aCommand.erase = [](Command& c) {};
	};
	
	std::function<void(Command& aCommand)> hide = [&](Command& aCommand)
	{
		// Delete
		if (!myEngine)
			return;
		if (aCommand.sceneID == -1)
			return;
		if (aCommand.entityID == -1)
			return;
		ServiceLocator& serviceLocator = myEngine->GetServiceLocator();
		SceneManager& sceneManager = serviceLocator.GetService<SceneManager>();
		Scene* scene = sceneManager.GetScene(aCommand.sceneID);
		if (!scene)
			return;
		if (!scene->HideEntity(aCommand.entityID))
			return;
		SetEntity(-1, -1);

		//aCommand.erase = [&](Command& aCommand2)
		//{
		//	// Recover
		//	if (!myEngine)
		//		return;
		//	if (aCommand2.sceneID == -1)
		//		return;
		//	if (aCommand2.entityID == -1)
		//		return;
		//	ServiceLocator& serviceLocator = myEngine->GetServiceLocator();
		//	SceneManager& sceneManager = serviceLocator.GetService<SceneManager>();
		//	Scene* scene = sceneManager.GetScene(aCommand2.sceneID);
		//	if (!scene)
		//		return;
		//	if (!scene->DestroyEntity(aCommand2.entityID))
		//		return;
		//};
	};
	
	if(aPerform)
	{
		hide(*c);
		c->revert = recover;
		c->redo = hide;
	}
	else
	{
		c->revert = hide;
		c->redo = recover;
	}
	myCommandQueue.Add(c);
}

void Inspector::EditInt(SerializableBase* base, CommandQueue& queue, const std::string& aSystemName)
{
	ImGui::PushID(base);
	auto ptr = reinterpret_cast<Serializable<int>*>(base);
	auto data = ptr->Get();
	auto name = (ptr->IsOverride() ? "*" : "") + base->GetName();

	static std::unordered_map<EditorControls, std::function<bool()>> funcMap;
	if (funcMap.empty())
	{
		funcMap[EditorControls::NONE] = [] { return false; };
		funcMap[EditorControls::INPUT_INT] = [&] { return ImGui::InputInt(name.c_str(), &data); };
		funcMap[EditorControls::DEFAULT] = [&] { return ImGui::DragInt(name.c_str(), &data); };
	}

	auto itr = funcMap.find(base->GetEditorControls());
	if (((itr != funcMap.end() && itr->second) ? itr->second : funcMap[EditorControls::DEFAULT])())
		Edit(ptr, data, queue, aSystemName);
	Apply<int>(ptr);
	ImGui::PopID();
}

void Inspector::EditFloat(SerializableBase* base, CommandQueue& queue, const std::string& aSystemName)
{
	ImGui::PushID(base);
	auto ptr = reinterpret_cast<Serializable<float>*>(base);
	auto data = ptr->Get();
	auto name = (ptr->IsOverride() ? "*" : "") + base->GetName();

	static std::unordered_map<EditorControls, std::function<bool()>> funcMap;
	if (funcMap.empty())
	{
		funcMap[EditorControls::NONE] = [] { return false; };
		funcMap[EditorControls::INPUT_FLOAT] = [&] { return ImGui::InputFloat(name.c_str(), &data); };
		funcMap[EditorControls::DEFAULT] = [&] { return ImGui::DragFloat(name.c_str(), &data); };
	}

	auto itr = funcMap.find(base->GetEditorControls());
	if (((itr != funcMap.end() && itr->second) ? itr->second : funcMap[EditorControls::DEFAULT])())
		Edit(ptr, data, queue, aSystemName);
	Apply<float>(ptr);
	ImGui::PopID();
}

void Inspector::EditBool(SerializableBase* base, CommandQueue& queue, const std::string& aSystemName)
{
	ImGui::PushID(base);
	auto ptr = reinterpret_cast<Serializable<bool>*>(base);
	auto data = ptr->Get();
	auto name = (ptr->IsOverride() ? "*" : "") + base->GetName();

	static std::unordered_map<EditorControls, std::function<bool()>> funcMap;
	if (funcMap.empty())
	{
		funcMap[EditorControls::NONE] = [] { return false; };
		funcMap[EditorControls::BUTTON] = [&] { data = ImGui::Button(name.c_str()); return data; };
		funcMap[EditorControls::DEFAULT] = [&] { return ImGui::Checkbox(name.c_str(), &data); };
	}
	auto itr = funcMap.find(base->GetEditorControls());
	if (((itr != funcMap.end() && itr->second) ? itr->second : funcMap[EditorControls::DEFAULT])())
		Edit(ptr, data, queue, aSystemName);
	Apply<bool>(ptr);
	ImGui::PopID();
}

void Inspector::EditString(SerializableBase* base, CommandQueue& queue, const std::string& aSystemName)
{
	ImGui::PushID(base);
	auto ptr = reinterpret_cast<Serializable<std::string>*>(base);
	auto data = ptr->Get();
	auto name = (ptr->IsOverride() ? "*" : "") + base->GetName();

	static std::unordered_map<EditorControls, std::function<bool()>> funcMap;
	if (funcMap.empty())
	{
		funcMap[EditorControls::NONE] = [] { return false; };
		funcMap[EditorControls::DEFAULT] = [&] { return ImGui::InputText(name.c_str(), &data); };
	}
	auto itr = funcMap.find(base->GetEditorControls());
	if (((itr != funcMap.end() && itr->second) ? itr->second : funcMap[EditorControls::DEFAULT])())
		Edit(ptr, data, queue, aSystemName);
	Apply<std::string>(ptr);
	ImGui::PopID();
}

void Inspector::EditVec2(SerializableBase* base, CommandQueue& queue, const std::string& aSystemName)
{
	ImGui::PushID(base);
	auto ptr = reinterpret_cast<Serializable<Vec2F>*>(base);
	auto data = ptr->Get();
	float val[2] = { data.x, data.y };
	auto name = (ptr->IsOverride() ? "*" : "") + base->GetName();
	static std::unordered_map<EditorControls, std::function<bool()>> funcMap;
	if (funcMap.empty())
	{
		funcMap[EditorControls::NONE] = [] { return false; };
		funcMap[EditorControls::INPUT_FLOAT] = [&]
		{
			if (ImGui::InputFloat2(name.c_str(), val))
			{
				data.x = val[0];
				data.y = val[1];
				ptr->Set(data);
				return true;
			}
			return false;
		};
		funcMap[EditorControls::DEFAULT] = [&]
		{
			if (ImGui::DragFloat2(name.c_str(), val))
			{
				data.x = val[0];
				data.y = val[1];
				ptr->Set(data);
				return true;
			}
			return false;
		};
	}
	auto itr = funcMap.find(base->GetEditorControls());
	if (((itr != funcMap.end() && itr->second) ? itr->second : funcMap[EditorControls::DEFAULT])())
		Edit(ptr, data, queue, aSystemName);
	Apply<Vec2F>(ptr);
	ImGui::PopID();
}

void Inspector::EditVec3(SerializableBase* base, CommandQueue& queue, const std::string& aSystemName)
{
	ImGui::PushID(base);
	auto ptr = reinterpret_cast<Serializable<Vec3F>*>(base);
	auto data = ptr->Get();
	float val[3] = { data.x, data.y, data.z };
	auto name = (ptr->IsOverride() ? "*" : "") + base->GetName();
	static std::unordered_map<EditorControls, std::function<bool()>> funcMap;
	if (funcMap.empty())
	{
		funcMap[EditorControls::NONE] = [] { return false; };
		funcMap[EditorControls::COLOR] = [&]
		{
			if (ImGui::ColorEdit3(name.c_str(), val))
			{
				data.x = val[0];
				data.y = val[1];
				data.z = val[2];
				ptr->Set(data);
				return true;
			}
			return false;
		};
		funcMap[EditorControls::INPUT_FLOAT] = [&]
		{
			if (ImGui::InputFloat3(name.c_str(), val))
			{
				data.x = val[0];
				data.y = val[1];
				data.z = val[2];
				ptr->Set(data);
				return true;
			}
			return false;
		};
		funcMap[EditorControls::DEFAULT] = [&]
		{
			if (ImGui::DragFloat3(name.c_str(), val))
			{
				data.x = val[0];
				data.y = val[1];
				data.z = val[2];
				ptr->Set(data);
				return true;
			}
			return false;
		};
	}
	auto itr = funcMap.find(base->GetEditorControls());
	if (((itr != funcMap.end() && itr->second) ? itr->second : funcMap[EditorControls::DEFAULT])())
		Edit(ptr, data, queue, aSystemName);
	Apply<Vec3F>(ptr);
	ImGui::PopID();
}

void Inspector::EditVec4(SerializableBase* base, CommandQueue& queue, const std::string& aSystemName)
{
	ImGui::PushID(base);
	auto ptr = reinterpret_cast<Serializable<CommonUtilities::Vector4<float>>*>(base);
	auto data = ptr->Get();
	float val[4] = { data.x, data.y, data.z, data.w };
	auto name = (ptr->IsOverride() ? "*" : "") + base->GetName();
	static std::unordered_map<EditorControls, std::function<bool()>> funcMap;
	if (funcMap.empty())
	{
		funcMap[EditorControls::NONE] = [] { return false; };
		funcMap[EditorControls::COLOR] = [&]
		{
			if (ImGui::ColorEdit4(name.c_str(), val))
			{
				data.x = val[0];
				data.y = val[1];
				data.z = val[2];
				data.z = val[3];
				ptr->Set(data);
				return true;
			}
			return false;
		};
		funcMap[EditorControls::INPUT_FLOAT] = [&]
		{
			if (ImGui::InputFloat4(name.c_str(), val))
			{
				data.x = val[0];
				data.y = val[1];
				data.z = val[2];
				data.z = val[3];
				ptr->Set(data);
				return true;
			}
			return false;
		};
		funcMap[EditorControls::DEFAULT] = [&]
		{
			if (ImGui::DragFloat4(name.c_str(), val))
			{
				data.x = val[0];
				data.y = val[1];
				data.z = val[2];
				data.z = val[3];
				ptr->Set(data);
				return true;
			}
			return false;
		};
	}
	auto itr = funcMap.find(base->GetEditorControls());
	if (((itr != funcMap.end() && itr->second) ? itr->second : funcMap[EditorControls::DEFAULT])())
		Edit(ptr, data, queue, aSystemName);
	Apply<CommonUtilities::Vector4<float>>(ptr);
	ImGui::PopID();
}