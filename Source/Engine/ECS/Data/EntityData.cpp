#include "EntityData.h"
#include "PrefabData.h"

EntityData::EntityData() : myEntity(nullptr)
{
}

EntityData::EntityData(const EntityData& anOther)
{
	myEntity = anOther.myEntity;
	mySerializables.clear();
	auto stored = SerializableBase::GetStored();
	for (auto& it : stored)
	{
		mySerializables[it->GetName()] = it;
	}
}

void EntityData::Create(PrefabData& somePrefabData)
{
	auto stored = SerializableBase::GetStored();
	for(auto& it : stored)
	{
		mySerializables[it->GetName()] = it;
		it->SetStored(somePrefabData);
	}
}

void EntityData::Serialize(rapidjson::Writer<rapidjson::StringBuffer>& aBase)
{
	for (auto& it : mySerializables)
		it.second->Serialize(aBase);
}

void EntityData::Deserialize(const rapidjson::Value::Object& aContainer)
{
	for (auto& it : mySerializables)
		it.second->Deserialize(aContainer);
}

std::unordered_map<std::string, SerializableBase*>& EntityData::GetSerializable()
{
	return mySerializables;
}

Entity* EntityData::GetEntity() const
{
	return myEntity;
}

void EntityData::SetEntity(Entity* anEntity)
{
	myEntity = anEntity;
}
