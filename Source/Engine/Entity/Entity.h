#pragma once
#include "Prefab.h"
#include "Transform.h"
#include "../RapidJSON/writer.h"
#include "../ResouceIDs.h"
#include <typeindex>

class Entity
{
	friend class Scene;
public:
	EntityID GetID() const;
	PrefabID GetPrefabID() const;
	Transform& GetTransform();
	void Save(rapidjson::Writer<rapidjson::StringBuffer>& aBase) const;
	bool Destroy();

private:
	void AddSystem(const std::type_index& aTypeIndex);
	void Construct(const EntityID anID, const PrefabID aPrefabID, const SceneID aSceneID);
	void Destruct();
	EntityID myID;
	PrefabID myPrefabID;
	SceneID mySceneID;
	Transform myTransform;
	std::vector<std::type_index> mySystemRefs;
};

