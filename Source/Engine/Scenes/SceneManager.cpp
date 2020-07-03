#include "SceneManager.h"
#include "Engine/EngineResources/ResourceManager.h"
#include "RapidJSON/document.h"

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
		if (e) c++;
	}
	Debug::Log << "Scene loaded, " << c << " instance" << (c > 1 ? "s" : "") << " created" << std::endl;
	return id;
}

