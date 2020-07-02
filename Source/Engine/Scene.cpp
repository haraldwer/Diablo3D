#include "Scene.h"
#include <iostream>
#include "Log.h"

Scene::Scene(SceneID anID, const std::string& aPath) : myID(anID), myPath(aPath)
{
}

Entity* Scene::CreateEntity()
{
	if (myLatestIndex < 0)
		myLatestIndex = 0;

	while (myObjectPool[myLatestIndex].myInUse)
	{
		myLatestIndex++;
		if (myLatestIndex >= poolSize)
		{
			Debug::Log << "Pool size is too small, object can't be created" << std::endl;
			return nullptr;
		}
	}
	myObjectPool[myLatestIndex].myObject.Construct(myLatestIndex);
	Entity* object = &myObjectPool[myLatestIndex].myObject;
	return object;
}

Entity* Scene::GetEntity(const EntityID anID)
{
	if (anID < 0 || anID >= poolSize || !myObjectPool[anID].myInUse)
	{
		Debug::Log << "Invalid ID" << std::endl;
		return nullptr;
	}
	return &myObjectPool[anID].myObject;
}

void Scene::DestroyEntity(const EntityID anID)
{
	if (anID < 0 || anID >= poolSize || !myObjectPool[anID].myInUse)
	{
		Debug::Log << "Invalid ID" << std::endl;
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
