#pragma once
#include <string>

#include "Entity.h"

typedef int SceneID;

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
	Entity* CreateEntity();
	Entity* GetEntity(EntityID anID);
	void DestroyEntity(EntityID anID);

	std::string GetPath() const;

private:
	SceneID myID;
	const std::string myPath;
	static const unsigned short poolSize = 100;
	EntitySlot myObjectPool[poolSize];
	int myLatestIndex;
};

