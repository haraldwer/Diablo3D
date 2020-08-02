#pragma once
#include "SystemBase.h"
#include "Data/Container.h"
#include "Data/Serializable.h"
#include "Data/EntityData.h"

template <class CS, class EntityData>
class System : public SystemBase
{
public:
	virtual ~System() = default;
	Container<EntityData>& GetContainer(); // Because of inheritance
	std::unordered_map<std::string, SerializableBase*>& GetEntityProperties(EntityID anID) override;
	std::unordered_map<std::string, StoredBase*>& GetPrefabProperties(PrefabID anID) override;
	SerializableBase* GetEntityProperty(EntityID anID, const std::string& aPropertyName) override;
	PrefabData& GetPrefabData(PrefabID aPrefabID) override;
	
private:
	
	// GetName, Init and Update are Pure Virtual and has to be defined in each system
	// Init() override;
	// Update() override;
	virtual std::string GetName() override = 0;

	// This function is called if the game isn't running but the editor is open
	virtual void EditorUpdate() override {}
	
	// Override these functions to write your own Editor UI using ImGui
	virtual void EditSystem() override {}
	virtual void EditPrefab(const PrefabID anID) override {}
	virtual void EditEntity(const EntityID anID) override {}
	
	// Override these if you want to change the behaviour when adding, or removing
	virtual void AddEntity(Entity* anEntity) override;
	virtual void RemoveEntity(EntityID anEntityID) override;
	virtual void SetEntityEnabled(const EntityID anEntityID, bool aEnabled) override;

	// These should not be overriden
	void ApplyEntityOverrides(EntityID anEntityID, const rapidjson::GenericObject<false, rapidjson::Value>& aBase) override;
	void GetEntityOverrides(EntityID anEntityID, rapidjson::Writer<rapidjson::StringBuffer>& aBase) override;
	void LoadPrefab(PrefabID aPrefabID, const rapidjson::GenericObject<false, rapidjson::Value>& aBase) override;
	size_t GetEntityCount() override;
	std::type_index GetTypeIndex() override;
	
	Container<EntityData> myContainer;
};

template <class CS, class EntityData>
Container<EntityData>& System<CS, EntityData>::GetContainer()
{
	return myContainer;
}

template <class CS, class EntityData>
std::unordered_map<std::string, SerializableBase*>& System<CS, EntityData>::GetEntityProperties(EntityID anID)
{
	auto& data = myContainer.GetData(anID);
	return data.GetSerializable();
}

template <class CS, class EntityData>
std::unordered_map<std::string, StoredBase*>& System<CS, EntityData>::GetPrefabProperties(PrefabID anID)
{
	auto& data = myContainer.GetPrefab(anID);
	return data.GetAllStored();
}

template <class CS, class EntityData>
SerializableBase* System<CS, EntityData>::GetEntityProperty(EntityID anID, const std::string& aPropertyName)
{
	auto& data = myContainer.GetData(anID);
	auto& serializable = data.GetSerializable();
	auto ptr = serializable.find(aPropertyName);
	if (ptr == serializable.end())
		return nullptr;
	return ptr->second;
}

template <class CS, class EntityData>
PrefabData& System<CS, EntityData>::GetPrefabData(PrefabID aPrefabID)
{
	return myContainer.GetPrefab(aPrefabID);
}

template <class CS, class EntityData>
void System<CS, EntityData>::AddEntity(Entity* anEntity)
{
	if(anEntity)
		auto& data = myContainer.Insert(*anEntity);
 }

template <class CS, class EntityData>
void System<CS, EntityData>::ApplyEntityOverrides(EntityID anEntityID, const rapidjson::GenericObject<false, rapidjson::Value>& aBase)
{
	if(aBase.HasMember(GetName().c_str()) && aBase[GetName().c_str()].IsObject())
	{
		auto& data = myContainer.GetData(anEntityID);
		data.Deserialize(aBase[GetName().c_str()].GetObject());
	}
}

template <class CS, class EntityData>
void System<CS, EntityData>::GetEntityOverrides(EntityID anEntityID, rapidjson::Writer<rapidjson::StringBuffer>& aBase)
{
	aBase.Key(GetName().c_str());
	aBase.StartObject();
	auto& data = myContainer.GetData(anEntityID);
	data.Serialize(aBase);
	aBase.EndObject();
}

template <class CS, class EntityData>
void System<CS, EntityData>::RemoveEntity(EntityID anEntityID)
{
	myContainer.Remove(anEntityID);
}

template <class CS, class EntityData>
void System<CS, EntityData>::SetEntityEnabled(const EntityID anEntityID, bool aEnabled)
{
	myContainer.SetEnabled(anEntityID, aEnabled);
}

template <class CS, class EntityData>
void System<CS, EntityData>::LoadPrefab(PrefabID aPrefabID, const rapidjson::GenericObject<false, rapidjson::Value>& aBase)
{
	PrefabData data;
	data.Deserialize(aBase);
	GetContainer().InsertPrefab(aPrefabID, data);
}

template <class CS, class EntityData>
size_t System<CS, EntityData>::GetEntityCount()
{
	return myContainer.GetEntityCount();
}

template <class CS, class EntityData>
std::type_index System<CS, EntityData>::GetTypeIndex()
{
	return std::type_index(typeid(System));
}

