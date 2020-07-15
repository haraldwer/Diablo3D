#pragma once
#include "Prefab.h"
#include "Transform.h"
#include "../RapidJSON/writer.h"

typedef int EntityID;

class Entity
{
	friend class Scene;
public:
	EntityID GetID() const;
	PrefabID GetPrefabID() const;
	Transform& GetTransform();
	void Save(rapidjson::Writer<rapidjson::StringBuffer>& aBase) const;
	
private:
	void Construct(const EntityID anID, const PrefabID aPrefabID);
	void Destruct();
	EntityID myID; // Duplicate entity ids are illegal
	PrefabID myPrefabID; // Duplicate prefab ids are illegal
	Transform myTransform;
};

