#include "Hierarchy.h"
#include <string>

bool Hierarchy::myIsFocused;

Hierarchy::Hierarchy(Inspector& anInspector) : myInspector(anInspector)
{
}

void Hierarchy::Init()
{
}

static auto vector_getter = [](void* vec, int idx, const char** out_text)
{
	auto& vector = *static_cast<std::vector<std::string>*>(vec);
	if (idx < 0 || idx >= static_cast<int>(vector.size())) { return false; }
	*out_text = vector.at(idx).c_str();
	return true;
};

bool ListBox(const char* label, int* currIndex, std::vector<std::string>& values)
{
	if (values.empty()) { return false; }
	return ImGui::ListBox(label, currIndex, vector_getter,
		static_cast<void*>(&values), values.size(), values.size());
}

void Hierarchy::Update(Engine* anEngine)
{
	if (!anEngine)
		return;

	myIsFocused = ImGui::IsWindowFocused();
	
	SceneManager& sceneManager = anEngine->GetServiceLocator().GetService<SceneManager>();
	ResourceManager& resourceManager = anEngine->GetServiceLocator().GetService<ResourceManager>();
	auto loadedScenes = sceneManager.GetLoadedScenes();

	bool br = false;
	
	for (auto& sceneID : loadedScenes)
	{
		auto scene = sceneManager.GetScene(sceneID);
		if (!scene)
		{
			ImGui::Text(("Unable to get scene with id " + std::to_string(sceneID)).c_str());
			continue;
		}
		auto resource = resourceManager.GetResource(scene->GetID());
		if (!resource)
		{
			ImGui::Text(("Unable to get scene resource with id " + std::to_string(scene->GetID())).c_str());
			continue;
		}
		const auto sceneName = resource->myName.c_str();
		if (ImGui::CollapsingHeader(sceneName, ImGuiTreeNodeFlags_DefaultOpen))
		{
			std::vector<EntityID> idVec;

			auto entities = scene->GetEntities();
			for (auto& entityID : entities)
			{
				Entity* entity = scene->GetEntity(entityID);
				std::string name = "";
				if (!entity)
				{
					name = "Unable to get entity with id " + std::to_string(entityID);
				}
				else
				{
					name = entity->GetName();
					if(name == "")
					{
						if(entity->GetPrefabID() != -1)
						{
							auto prefabResource = resourceManager.GetResource(entity->GetPrefabID());
							if (!prefabResource)
							{
								name = "Unable to get prefab resource with id " + std::to_string(entity->GetPrefabID());
							}
							else 
								name = prefabResource->myName;
						}
						if(name == "")
							name = "Unnamed entity";
					}
				}

				const bool open = ImGui::TreeNodeEx((name + "##" + std::to_string(entityID)).c_str(), myInspector.GetIsSelected(sceneID, entityID) ? ImGuiTreeNodeFlags_Bullet : ImGuiTreeNodeFlags_Leaf);

				if (ImGui::BeginPopupContextItem((name + "##" + std::to_string(entityID)).c_str()))
				{
					myInspector.SetEntity(sceneID, entityID);
					if (ImGui::MenuItem("Duplicate"))
						myInspector.Duplicate();
					if (ImGui::MenuItem("Delete"))
						myInspector.Delete();
					if (ImGui::MenuItem("Create empty"))
						CreateEmpty(sceneID, anEngine);
					if (ImGui::MenuItem("Unload scene"))
					{
						sceneManager.UnloadScene(sceneID);
						br = true;
					}
					ImGui::EndPopup();
				}
				
				if(open)
					ImGui::TreePop();
				
				if (ImGui::IsItemClicked())
					myInspector.SetEntity(sceneID, entityID);

				if (br)
					break;
			}
		}
		DragDrop(sceneID, -1, anEngine);
		if (br)
			break;
	}

	if(myIsFocused)
	{
		Input& input = anEngine->GetServiceLocator().GetService<Input>();
		if(input.Get(VK_CONTROL) && input.GetPressed('S'))
		{
			for (auto& sceneID : loadedScenes)
			{
				auto scene = sceneManager.GetScene(sceneID);
				if (!scene)
				{
					ImGui::Text(("Unable to get scene with id " + std::to_string(sceneID)).c_str());
					continue;
				}
				scene->Save();
			}
		}
	}
}

void Hierarchy::DragDrop(SceneID aSceneID, EntityID anEntityID, Engine* anEngine)
{
	if (ImGui::BeginDragDropTarget())
	{
		ImGuiDragDropFlags target_flags = 0;
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("RESOURCE", target_flags))
		{
			ResourceID id = *(ResourceID*)payload->Data;
			auto& resourceManager = anEngine->GetServiceLocator().GetService<ResourceManager>();
			auto resource = resourceManager.GetResource(id);
			if(resource)
			{
				if(resource->myType._value == ResourceType::PREFAB)
				{
					auto& prefabManager = anEngine->GetServiceLocator().GetService<PrefabManager>();
					auto prefab = prefabManager.GetPrefab(resource->myID);
					if(prefab)
					{
						auto& sceneMan = anEngine->GetServiceLocator().GetService<SceneManager>();
						Scene* scene = sceneMan.GetScene(aSceneID);
						if (scene)
						{
							Entity* entity = scene->CreateEntity(resource->myID);
						}
					}
				}
			}
		}
		ImGui::EndDragDropTarget();
	}
}

void Hierarchy::CreateEmpty(SceneID aSceneID, Engine* anEngine)
{
	auto& sceneMan = anEngine->GetServiceLocator().GetService<SceneManager>();
	Scene* scene = nullptr;
	scene = sceneMan.GetScene(aSceneID);
	if (!scene)
		return;
 	Entity* entity = scene->CreateEntity();
	if (!entity)
		Debug::Error << "Unable to create entity" << std::endl;
}

bool Hierarchy::IsFocused()
{
	return myIsFocused;
}
