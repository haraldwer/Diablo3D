#pragma once
#include "SerializableBase.h"
#include <unordered_map>

class PrefabData;
class Entity;

class EntityData
{
public:
	EntityData();
	virtual ~EntityData() = default;
	EntityData(const EntityData& anOther);
	void Create(PrefabData& somePrefabData);
	void Serialize(rapidjson::Writer<rapidjson::StringBuffer>& aBase);
	void Deserialize(const rapidjson::Value::Object& aContainer);
	std::unordered_map<std::string, SerializableBase*>& GetSerializable();
	Entity* GetEntity() const;
	void SetEntity(Entity* anEntity);
private:
	std::unordered_map<std::string, SerializableBase*> mySerializables;
	Entity* myEntity;
};