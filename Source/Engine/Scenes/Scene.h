#pragma once
#include <string>
#include "../Entity/Entity.h"
#include "../../RapidJSON/document.h"
#include "../ResouceIDs.h"

struct EntitySlot
{
	EntitySlot() : myObject(), myInUse(false)
	{
	}
	Entity myObject;
	bool myInUse;
};

class Scene
{
public:
	Scene(SceneID anID, const std::string& aPath);
	Entity* CreateEntity(PrefabID aPrefabID);
	Entity* GetEntity(EntityID anID);
	bool DestroyEntity(EntityID anID);
	std::string GetPath() const;
	SceneID GetID() const;
	std::vector<EntityID> GetEntities();
	
	void Load(rapidjson::Document& aDoc);
	void Save();
	
private:
	const SceneID myID;
	const std::string myPath;
	static const unsigned short poolSize = 1000;
	EntitySlot myObjectPool[poolSize];
	int myLatestIndex;
};

