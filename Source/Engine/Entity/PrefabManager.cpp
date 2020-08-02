#include "PrefabManager.h"
#include "Prefab.h"
#include "Engine/ECS/System.h"
#include "Engine/ECS/CSystemManager.h"
#include "Engine/EngineResources/EngineResource.h"
#include "Engine/EngineResources/ResourceManager.h"
#include "RapidJSON/document.h"
#include "ImGui/imgui.h"
#include <fstream>
#include "../RapidJSON/formatter.h"

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

PrefabID PrefabManager::GetPrefabID(const std::string& aName)
{
	const auto find = myPrefabNames.find(aName);
	if(find == myPrefabNames.end())
	{
		Debug::Warning << "Unable to find prefab with name " << aName << std::endl;
		return -1;
	}
	return find->second;
}

void PrefabManager::Save()
{
	auto& resourceManager = ServiceLocator::Instance().GetService<ResourceManager>();
	auto& sysMan = ServiceLocator::Instance().GetService<CSystemManager>();
	for(auto& it : myPrefabs)
	{
		if(!it.second)
			continue;

		auto resource = resourceManager.GetResource(it.first);
		if(!resource)
		{
			Debug::Error << "Unable to save prefab, unable to find resource with ID " << it.first << std::endl;
			continue;
		}
		
		rapidjson::StringBuffer s;
		rapidjson::Writer<rapidjson::StringBuffer> writer(s);
		writer.StartObject();
		if (!resourceManager.SaveResource(it.first, writer))
			continue;
		
		writer.Key("Components");
		writer.StartArray();
		auto comps = it.second->GetComponents();
		for(auto& comp : comps)
		{
			auto sys = sysMan.GetSystem(comp);
			if (!sys)
			{
				Debug::Error << "Unable to find CSystem " << comp << " when trying to save prefab " << it.second->GetName() << std::endl;
				continue;
			}
			writer.StartObject();
			writer.Key("System");
			writer.String(comp.c_str());
			auto& data = sys->GetPrefabData(it.first);
			data.Serialize(writer);
			writer.EndObject();
		}
		writer.EndArray();
		writer.EndObject();
		const std::string formatted = Format(s.GetString());
		std::ofstream out(resource->myPath);
		out.clear();
		out << formatted;
		out.close();
		if (resource)
			resource->myDoc = formatted;
		Debug::Log << "Prefab " << resource->myName << " saved" << std::endl;
	}
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
	myPrefabNames[prefab->myName] = id;
	myPrefabs[id] = prefab;
}
