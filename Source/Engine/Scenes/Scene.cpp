#include "Scene.h"
#include "Log.h"
#include "Engine/ECS/CSystemManager.h"
#include "Engine/Utility/ServiceLocator.h"
#include "Engine/Entity/PrefabManager.h"
#include "Engine/ECS/CSystem.h"
#include "../Utility/JSONHelper.h"
#include "Engine/EngineResources/ResourceManager.h"
#include "RapidJSON/formatter.h"
#include <fstream>

Scene::Scene(SceneID anID, const std::string& aPath) : myID(anID), myPath(aPath), myLatestIndex(0)
{
}

Entity* Scene::CreateEntity(PrefabID aPrefabID)
{
	Prefab* prefab = ServiceLocator::Instance().GetService<PrefabManager>().GetPrefab(aPrefabID);
	if(prefab == nullptr)
	{
		Debug::Error << "Unable to find prefab with ID " << aPrefabID << std::endl;
		return nullptr;
	}
	
	if (myLatestIndex < 0)
		myLatestIndex = 0;

	while (myObjectPool[myLatestIndex].myInUse)
	{
		myLatestIndex++;
		if (myLatestIndex >= poolSize)
		{
			Debug::Warning << "Pool size is too small, object can't be created" << std::endl;
			return nullptr;
		}
	}
	myObjectPool[myLatestIndex].myInUse = true;
	myObjectPool[myLatestIndex].myObject.Construct(myLatestIndex, aPrefabID);
	Entity* entity = &myObjectPool[myLatestIndex].myObject;
	auto& systemManager = ServiceLocator::Instance().GetService<CSystemManager>();
	auto comp = prefab->GetComponents();
	for(auto& it : comp)
	{
		CSystemBase* system = systemManager.GetSystem(it);
		if(!system)
		{
			Debug::Error << "Unable to find system " << it << " for prefab " << prefab->GetName() << std::endl;
			continue;
		}
		system->AddEntity(entity);
	}
	return entity;
}

Entity* Scene::GetEntity(const EntityID anID)
{
	if (anID < 0 || anID >= poolSize || !myObjectPool[anID].myInUse)
	{
		Debug::Warning << "Invalid entity ID" << std::endl;
		return nullptr;
	}
	return &myObjectPool[anID].myObject;
}

void Scene::DestroyEntity(const EntityID anID)
{
	if (anID < 0 || anID >= poolSize || !myObjectPool[anID].myInUse)
	{
		Debug::Warning << "Invalid ID" << std::endl;
		return;
	}
	myObjectPool[anID].myObject.Destruct();
	myObjectPool[anID].myInUse = false;
	if (anID < myLatestIndex)
		myLatestIndex = anID;
}

std::string Scene::GetPath() const
{
	return myPath;
}

SceneID Scene::GetID() const
{
	return myID;
}

std::vector<EntityID> Scene::GetEntities()
{
	std::vector<EntityID> entities;
	for (auto& it : myObjectPool)
		if (it.myInUse)
			entities.push_back(it.myObject.GetID());
	return entities;
}

void Scene::Load(rapidjson::Document& aDoc)
{
	int c = 0;
	for (auto& it : aDoc["Entities"].GetArray())
	{
		if (!it.IsObject() || !it.HasMember("Prefab") || !it["Prefab"].IsInt())
		{
			Debug::Error << "Prefab reference is missing or formatted incorrectly in \"" << myPath << "\"" << std::endl;
			continue;
		}

		const auto e = CreateEntity(it["Prefab"].GetInt());
		if (e)
		{
			e->GetTransform().SetMatrix(Deserialize::Matrix(it.GetObject(), "Transform"));
			c++;
		}
	}
	Debug::Log << "Scene loaded, " << c << " instance" << (c > 1 ? "s" : "") << " created" << std::endl;
}

void Scene::Save()
{
	auto& resourceManager = ServiceLocator::Instance().GetService<ResourceManager>();
	rapidjson::StringBuffer s;
	rapidjson::Writer<rapidjson::StringBuffer> writer(s);
	writer.StartObject();
	if(resourceManager.SaveResource(myID, writer))
	{
		writer.Key("Entities");
		writer.StartArray();
		for(auto& it : myObjectPool)
			if(it.myInUse)
				it.myObject.Save(writer);
		writer.EndArray();
		writer.EndObject();
		const std::string formatted = Format(s.GetString());
		std::ofstream out(myPath);
		out.clear();
		out << formatted;
		out.close();
		auto resource = resourceManager.GetResource(myID);
		if (resource)
			resource->myDoc = formatted;
		Debug::Log << "Scene " << resource->myName << " saved" << std::endl;
	}
	else
	{
		writer.EndObject();
	}
}
