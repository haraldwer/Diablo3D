#include "PrefabManager.h"
#include "Prefab.h"
#include "Engine/ECS/CSystem.h"
#include "Engine/ECS/CSystemManager.h"
#include "Engine/EngineResources/EngineResource.h"
#include "Engine/EngineResources/ResourceManager.h"
#include "RapidJSON/document.h"
#include "ImGui/imgui.h"

void PrefabManager::Init()
{
	Load();
}

void PrefabManager::Editor()
{
	ImGui::Text("Prefabs");
	ImGui::Separator();
	for(auto& it : myPrefabs)
	{
		ImGui::BeginChild(it.second->myName.c_str());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 1));
		ImGui::Text((it.second->myName + " (" + std::to_string(it.second->myID) + ")" + ":").c_str());
		for(auto& system : it.second->myComponents)
		{
			ImGui::Text((" - " + system).c_str());
		}
		ImGui::PopStyleVar();
		ImGui::EndChild();
	}
}

Prefab* PrefabManager::GetPrefab(PrefabID aPrefabID)
{
	if (myPrefabs.find(aPrefabID) == myPrefabs.end())
	{
		Debug::Error << "Unable to find prefab with ID: " << aPrefabID;
		return nullptr;
	}
	return myPrefabs[aPrefabID];
}

void PrefabManager::Load()
{
	auto prefabs = ServiceLocator::Instance().GetService<ResourceManager>().GetResources(ResourceType::PREFAB);
	for(auto& it : prefabs)
	{
		LoadPrefab(it);
	}
	Debug::Log << myPrefabs.size() << " prefab" << (myPrefabs.size() > 1 ? "s" : "") << " has been loaded" << std::endl;
}

void PrefabManager::LoadPrefab(EngineResource* resource)
{
	if(!resource)
	{
		Debug::Error << "Prefab resource was nullptr" << std::endl;
		return;
	}
	if(resource->myDoc.empty())
	{
		Debug::Error << resource->myPath << " : Prefab resource doc was empty " << std::endl;
		return;
	}
	rapidjson::Document doc;
	doc.Parse(resource->myDoc.c_str());
	if(!doc.IsObject())
	{
		Debug::Error << resource->myPath << " : Json not formatted properly" << std::endl;
		return;
	}
	if(!doc.HasMember("Components"))
	{
		Debug::Error << resource->myPath << " : Missing Components-array in Json" << std::endl;
		return;
	}
	if(!doc["Components"].IsArray())
	{
		Debug::Error << resource->myPath << " : Components-array in Json is not formatted properly" << std::endl;
		return;
	}

	PrefabID id = resource->myID;
	if(myPrefabs.find(id) != myPrefabs.end())
	{
		Debug::Error << resource->myName << " : A prefab with the ID " << id << " has already been loaded" << std::endl;
		return;
	}
	
	std::vector<std::string> systems;
	auto arr = doc["Components"].GetArray();
	for(auto& it : arr)
	{
		if(!it.IsObject() || !it.HasMember("System") || !it["System"].IsString())
		{
			Debug::Error << resource->myName << " : Component array object did not contain system reference or was formatted incorrectly" << std::endl;
			continue;
		}
		std::string system = it["System"].GetString();
		systems.push_back(system);
		auto ptr = ServiceLocator::Instance().GetService<CSystemManager>().GetSystem(system);
		if(!ptr)
		{
			Debug::Error << resource->myName << " : Unable to find component system " << system << std::endl;
			continue;
		}
		ptr->LoadPrefab(id, it.GetObject());
	}
	
	Prefab* prefab = new Prefab();
	prefab->myID = id;
	prefab->myComponents = systems;
	prefab->myName = resource->myName;
	myPrefabs[id] = prefab;
}
