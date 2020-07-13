#include "ResourceManager.h"
#include "Log.h"
#include "RapidJSON/parser.h"
#include "RapidJSON/document.h"
#include <filesystem>

#include "HashMap.hpp"
#include "ImGui/imgui.h"

namespace fs = std::filesystem;

void ResourceManager::LoadResources(const std::string& aPath)
{
	Debug::Log << "Loading resources" << std::endl;
	for (auto& it : myResources)
		delete it.second;
	myResources.clear();
	myResourceTypeMap.clear();
	if(myFolder)
	{
		delete(myFolder);
		myFolder = nullptr;
	}
	if(!myFolder)
	{
		myFolder = new Folder(aPath);
	}
	LoadResourcesRec(aPath, myFolder);
}

void ResourceManager::LoadResourcesRec(const std::string& aPath, Folder* aFolder)
{
	if(!aFolder)
	{
		Debug::Error << "Folder was nullptr when loading resources" << std::endl;
		return;
	}
	for (const auto& entry : fs::directory_iterator(aPath))
	{
		if (entry.is_directory())
		{
			Folder* f = new Folder(entry.path().string());
			aFolder->folders.push_back(f);
			LoadResourcesRec(entry.path().string(), f);
			continue;
		}

		auto ext = GetExt(entry.path().string());
		if (ext == std::string("json"))
			LoadResource(entry.path().string(), aFolder);
		else // If resource hasnt already been loaded
		{
			bool res = false;
			for (auto& it : aFolder->resources)
			{
				if (ClipExt(it->myPath) == ClipExt(entry.path().string()))
				{
					res = true;
					break;
				}
			}
			if(!res)
				aFolder->unloadedResources.push_back(new UnloadedResource(entry.path().string()));
		}
	}
}

void ResourceManager::LoadResource(const std::string& aPath, Folder* aFolder)
{
	std::string doc = Parse(aPath);
	rapidjson::Document json;
	json.Parse(doc.c_str());
	if (!json.IsObject())
	{
		Debug::Error << "Unable to load resource: " << aPath << ". Resource isn't a valid JSON." << std::endl;
		return;
	}

	if (!json.HasMember("Name") || !json["Name"].IsString())
	{
		Debug::Error << "Unable to load resource: " << aPath << ". Resource name is missing in JSON." << std::endl;
		return;
	}
	if (!json.HasMember("Type") || !json["Type"].IsString())
	{
		Debug::Error << "Unable to load resource: " << aPath << ". Resource type is missing in JSON." << std::endl;
		return;
	}
	if (!json.HasMember("ID") || !json["ID"].IsInt())
	{
		Debug::Error << "Unable to load resource: " << aPath << ". Resource id is missing in JSON." << std::endl;
		return;
	}
	if (!json.HasMember("Ext") || !json["Ext"].IsString())
	{
		Debug::Error << "Unable to load resource: " << aPath << ". Resource ext is missing in JSON." << std::endl;
		return;
	}
	
	std::string type = json["Type"].GetString();
	if(!ResourceType::_is_valid(type.c_str()))
	{
		Debug::Error << "Unable to load resource: " << aPath << ". Unknown resource type " << type << std::endl;
		return;
	}

	ResourceID id = json["ID"].GetInt();
	if (myResources.find(id) != myResources.end())
	{
		Debug::Error << "Unable to load resource: " << aPath << ". A resource with ID " << id << " already exists. " << std::endl;
		return;
	}

	ResourceType eType = ResourceType::_from_string(type.c_str());
	
	EngineResource* resource = new EngineResource();
	resource->myName = json["Name"].GetString();
	resource->myType = eType;
	resource->myDoc = doc;
	resource->myExt = json["Ext"].GetString();
	resource->myPath = ClipExt(aPath) + "." + resource->myExt;
	resource->myID = id;
	myResources[id] = resource;
	myResourceTypeMap[eType._to_integral()].push_back(resource);
	aFolder->resources.push_back(resource);
	for (int i = 0; i < aFolder->unloadedResources.size(); i++)
	{
		if (ClipExt(aFolder->unloadedResources[i]->path) == ClipExt(resource->myPath))
		{
			aFolder->unloadedResources.erase(aFolder->unloadedResources.begin() + i);
			break;
		}
	}
}

EngineResource* ResourceManager::CreateResource(const std::string& aResourceName, ResourceType aType)
{
	// Create hash
	ResourceID hash = CommonUtilities::Hash(aResourceName);
	while (myResources.find(hash) == myResources.end())
		hash++;
	EngineResource* resource = new EngineResource();
	myResources[hash] = resource;
	myResourceTypeMap[aType._to_integral()].push_back(resource);
	return resource;
}

EngineResource* ResourceManager::GetResource(ResourceID anID)
{
	auto itr = myResources.find(anID);
	if (itr == myResources.end())
		return nullptr;
	return itr->second;
}

std::vector<EngineResource*> ResourceManager::GetResources(ResourceType aType)
{
	return myResourceTypeMap[aType._to_integral()];
}

void ResourceManager::Editor()
{
	ImGui::Text("Resources:");
	ImGui::BeginChild("Resources");
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 1));
	for (auto& it : myResourceTypeMap)
	{
		for (auto& resource : it.second)
		{
			ImGui::Separator();
			ImGui::Text(("Name: " + resource->myName).c_str());
			ImGui::Text(("ID: " + std::to_string(resource->myID)).c_str());
			ImGui::Text(("Type: " + std::string(resource->myType._to_string())).c_str());
			ImGui::Text(("Path: " + resource->myPath).c_str());
			ImGui::Text(("Ext: " + resource->myExt).c_str());
		}
	}
	ImGui::PopStyleVar();
	ImGui::EndChild();
}

ResourceManager::Folder* ResourceManager::GetFolder() const
{
	return myFolder;
}
