#include "SceneManager.h"

#include "Horspool.hpp"
#include "Engine/EngineResources/ResourceManager.h"
#include "Engine/Physics/PhysicsManager.h"
#include "RapidJSON/document.h"
#include "ImGui/imgui.h"

void SceneManager::Init()
{
}

SceneID SceneManager::LoadScene(std::string aScene)
{
	auto resources = ServiceLocator::Instance().GetService<ResourceManager>().GetResources(ResourceType::SCENE);
	for(auto& it : resources)
	{
		if(it->myPath == aScene || it->myName == aScene)
		{
			return LoadScene(it);
		}
	}
	Debug::Error << "Could not find scene resource with name " << aScene << std::endl;
	return -1;
}

SceneID SceneManager::LoadScene(ResourceID sceneID)
{
	EngineResource* resource = ServiceLocator::Instance().GetService<ResourceManager>().GetResource(sceneID);
	return LoadScene(resource);
}

Vec3F LoadVec(const char* name, const rapidjson::GenericObject<false, rapidjson::Value>& aBase)
{
	Vec3F vec;
	if(aBase.HasMember(name) && aBase[name].IsObject())
	{
		if (aBase[name].HasMember("x"))
			if(aBase[name]["x"].IsFloat() || aBase[name]["x"].IsInt())
				vec.x = aBase[name]["x"].GetFloat();
		if (aBase[name].HasMember("y"))
			if (aBase[name]["y"].IsFloat() || aBase[name]["x"].IsInt())
				vec.y = aBase[name]["y"].GetFloat();
		if (aBase[name].HasMember("z"))
			if (aBase[name]["z"].IsFloat() || aBase[name]["x"].IsInt())
				vec.z = aBase[name]["z"].GetFloat();
	}
	return vec;
}

SceneID SceneManager::LoadScene(EngineResource* aSceneResource)
{
	if (!aSceneResource)
	{
		Debug::Error << "Scene resource was nullptr" << std::endl;
		return -1;
	}
	
	const std::string& path = aSceneResource->myPath;
	if (aSceneResource->myDoc.empty())
	{
		Debug::Error << path << " : Scene resource doc was empty " << std::endl;
		return -1;
	}
	
	rapidjson::Document doc;
	doc.Parse(aSceneResource->myDoc.c_str());
	if (!doc.IsObject())
	{
		Debug::Error << path << " : Json not formatted properly" << std::endl;
		return -1;
	}
	if (!doc.HasMember("Entities"))
	{
		Debug::Error << path << " : Missing Entities-array in Json" << std::endl;
		return -1;
	}
	if (!doc["Entities"].IsArray())
	{
		Debug::Error << path << " : Entities-array in Json is not formatted properly" << std::endl;
		return -1;
	}

	const SceneID id = aSceneResource->myID;
	if(myScenes.find(id) != myScenes.end())
	{
		Debug::Warning << path << " : Scene already loaded";
		return -1;
	}
	
	Scene* scene = new Scene(id, path);
	myScenes[id] = scene;
	int c = 0;
	for(auto& it : doc["Entities"].GetArray())
	{
		if(!it.IsObject() || !it.HasMember("Prefab") || !it["Prefab"].IsInt())
		{
			Debug::Error << aSceneResource->myName << " : Entity array object did not contain prefab reference or was formatted incorrectly" << system << std::endl;
			continue;
		}
		
		const auto e = scene->CreateEntity(it["Prefab"].GetInt());
		if (e)
		{
			e->GetTransform().SetPosition(LoadVec("Position", it.GetObject()));
			e->GetTransform().SetRotation(LoadVec("Rotation", it.GetObject()));
			e->GetTransform().SetScale(LoadVec("Scale", it.GetObject()));
			c++;
		}
	}
	Debug::Log << "Scene loaded, " << c << " instance" << (c > 1 ? "s" : "") << " created" << std::endl;
	return id;
}

bool SceneManager::UnloadScene(const std::string& aScene)
{
	for (auto& it : myScenes)
		if (CommonUtilities::Horspool(aScene, it.second->GetPath()) != -1)
			UnloadScene(it.second->GetID());
	return false;
}

bool SceneManager::UnloadScene(SceneID aSceneID)
{
	if(myScenes.find(aSceneID) == myScenes.end() || myScenes[aSceneID] == nullptr)
	{
		Debug::Warning << "Scene already destroyed. SceneID: " << aSceneID << std::endl;
		return false;
	}
	ServiceLocator::Instance().GetService<PhysicsManager>().DestroyScene(aSceneID);
	delete myScenes[aSceneID];
	myScenes[aSceneID] == nullptr;
	return true;
}

void SceneManager::Editor()
{
	ImGui::Text("Loaded scenes: ");
	ImGui::Separator();
	ImGui::BeginChild("SceneManager editor");	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 1));
	for (auto& it : myScenes)
		if (it.second)
			ImGui::Text((it.second->GetPath()).c_str());
	ImGui::PopStyleVar();
	ImGui::EndChild();
}

Scene* SceneManager::GetScene(SceneID aSceneID)
{
	auto itr = myScenes.find(aSceneID);
	if(itr == myScenes.end())
	{
		Debug::Error << "No scene with ID " << aSceneID << " is loaded. "<< std::endl;
		return nullptr;
	}
	return itr->second;
}

std::vector<SceneID> SceneManager::GetLoadedScenes()
{
	std::vector<SceneID> scenes;
	for (auto& it : myScenes)
		scenes.push_back(it.first);
	return scenes;
}
