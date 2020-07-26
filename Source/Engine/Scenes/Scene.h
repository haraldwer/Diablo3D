#pragma once
#include <string>
#include "../Entity/Entity.h"
#include "../../RapidJSON/document.h"
#include "../ResouceIDs.h"

struct EntitySlot
{
	enum class Status
	{
		EMPTY,
		USED,
		HIDDEN,
	};
	
	EntitySlot() : myObject(), myStatus(Status::EMPTY)
	{
	}
	Entity myObject;
	Status myStatus;
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
	bool HideEntity(EntityID anID);
	bool ShowEntity(EntityID anID);
	
	
	void Load(rapidjson::Document& aDoc);
	void Save();
	
private:
	const SceneID myID;
	const std::string myPath;
	static const unsigned short poolSize = 1000;
	EntitySlot myObjectPool[poolSize];
	int myLatestIndex;
};

