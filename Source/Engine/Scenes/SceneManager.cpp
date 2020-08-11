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
	ServiceLocator::Instance().GetService<PhysicsManager>().CreateScene(id);
	myScenes[id] = scene;
	scene->Load(doc);
	return id;
}

bool SceneManager::UnloadScene(const std::string& aScene)
{
	for (auto& it : myScenes)
		if (CommonUtilities::Horspool(aScene, it.second->GetPath()) != -1)
			return UnloadScene(it.second->GetID());
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
	myScenes.erase(aSceneID);
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

Scene* SceneManager::GetFirstScene()
{
	if (myScenes.empty())
		return nullptr;
	return myScenes.begin()->second;
}

std::vector<SceneID> SceneManager::GetLoadedScenes()
{
	std::vector<SceneID> scenes;
	for (auto& it : myScenes)
		scenes.push_back(it.first);
	return scenes;
}

void SceneManager::SaveScenes()
{
	for (auto& it : myScenes)
		it.second->Save();
}
