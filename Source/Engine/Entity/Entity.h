#pragma once
#include "Transform.h"
#include "../Utility/JSONHelper.h"
#include "../ResouceIDs.h"
#include <typeindex>

class Entity
{
	friend class Scene;
	friend class Inspector;
public:
	EntityID GetID() const;
	SceneID GetSceneID() const;
	PrefabID GetPrefabID() const;
	Transform& GetTransform();

	// Write to json
	void Serialize(rapidjson::Writer<rapidjson::StringBuffer>& aBase) const;
	// Load from json
	void Deserialize(const rapidjson::Value::Object& aContainer);

	// Will enable / disable this instance completely
	void SetEnabled(bool aEnabled);
	bool GetEnabled() const;
	bool Destroy();

	// List of systems where this entity has component data
	std::vector<std::type_index> GetSystemRefs() const;
	
private:
	void AddSystem(const std::type_index& aTypeIndex);
	void Construct(const EntityID anID, const PrefabID aPrefabID, const SceneID aSceneID);
	void Destruct();
	EntityID myID;
	PrefabID myPrefabID;
	SceneID mySceneID;
	Transform myTransform;
	bool myEnabled;
	std::vector<std::type_index> mySystemRefs;
};

