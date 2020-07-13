#include "Hierarchy.h"
#include <string>

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

	SceneManager& sceneManager = anEngine->GetServiceLocator().GetService<SceneManager>();
	ResourceManager& resourceManager = anEngine->GetServiceLocator().GetService<ResourceManager>();
	auto loadedScenes = sceneManager.GetLoadedScenes();
	
	for(auto& sceneID : loadedScenes)
	{
		auto scene = sceneManager.GetScene(sceneID);
		if (!scene)
		{
			ImGui::Text(("Unable to get scene with id " + std::to_string(sceneID)).c_str());
			continue;
		}
		auto resource = resourceManager.GetResource(scene->GetID());
		if(!resource)
		{
			ImGui::Text(("Unable to get scene resource with id " + std::to_string(scene->GetID())).c_str());
			continue;
		}
		const auto sceneName = resource->myName.c_str();
		if (ImGui::CollapsingHeader(sceneName, ImGuiTreeNodeFlags_DefaultOpen))
		{
			std::vector<std::string> nameVec;
			std::vector<EntityID> idVec;
			
			auto entities = scene->GetEntities();
			for(auto& entityID : entities)
			{
				Entity* entity = scene->GetEntity(entityID);
				std::string name = "";
				EntityID id = -1;
				if(!entity)
				{
					name = "Unable to get entity with id " + std::to_string(entityID);
					continue;
				}
				
				auto prefabResource = resourceManager.GetResource(entity->GetPrefabID());
				if(!prefabResource)
				{
					name = "Unable to get prefab resource with id " + std::to_string(entity->GetPrefabID());
					continue;
				}
				name = prefabResource->myName;
				id = entityID;
				if (ImGui::TreeNodeEx((name + "##" + std::to_string(id)).c_str(), ImGuiTreeNodeFlags_Leaf))
					ImGui::TreePop();
				if(ImGui::IsItemClicked())
				{
					Debug::Log << entityID << std::endl;
					myInspector.SetEntity(sceneID, entityID);
				}
			}
		}
	}
}
