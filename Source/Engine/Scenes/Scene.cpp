#include "Scene.h"
#include "Log.h"
#include "Engine/ECS/CSystemManager.h"
#include "Engine/Utility/ServiceLocator.h"
#include "Engine/Entity/PrefabManager.h"
#include "Engine/ECS/CSystem.h"

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
	myObjectPool[myLatestIndex].myObject.Construct(myLatestIndex, aPrefabID);
	Entity* entity = &myObjectPool[myLatestIndex].myObject;
	CSystemManager& systemManager = ServiceLocator::Instance().GetService<CSystemManager>();
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
		Debug::Warning << "Invalid ID" << std::endl;
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
