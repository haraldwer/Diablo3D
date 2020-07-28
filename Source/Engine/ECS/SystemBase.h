#pragma once
#include <string>
#include <typeindex>
#include "CSystemManager.h"
#include "../ResouceIDs.h"
#include "../../RapidJSON/document.h"
#include "../../RapidJSON/writer.h"

class Transform;
class Entity;
class SerializableBase;
class StoredBase;

class SystemBase
{
	friend class CSystemManager;
	friend class PrefabManager;
	friend class Scene;
	friend class Entity;
public:
	SystemBase() { CSystemManager::mySystems.push_back(this); }
	virtual ~SystemBase() = default;
	template <class T>
	static T* GetSystem();
	virtual std::string GetName() = 0;
	virtual std::unordered_map<std::string, SerializableBase*>& GetEntityProperties(EntityID anID) = 0;
	virtual std::unordered_map<std::string, StoredBase*>& GetPrefabProperties(PrefabID anID) = 0;
	virtual SerializableBase* GetEntityProperty(EntityID anID, const std::string& aPopertyName) = 0;
	
private:

	// Init, Update and GetName has to be defined in each system
	virtual void Init() = 0;
	virtual void Update() = 0;

	// Add, remove and load has a definition in System but can be overridden
	virtual void AddEntity(Entity* anEntity) = 0;
	virtual void ApplyEntityOverrides(EntityID anEntityID, const rapidjson::GenericObject<false, rapidjson::Value>& aBase) = 0;
	virtual void GetEntityOverrides(EntityID anEntityID, rapidjson::Writer<rapidjson::StringBuffer>& aBase) = 0;
	virtual void RemoveEntity(EntityID anEntityID) = 0;
	virtual void LoadPrefab(PrefabID, const rapidjson::GenericObject<false, rapidjson::Value>& aBase) = 0;

	// The rest are defined in System and cannot be overriden
	virtual std::type_index GetTypeIndex() = 0;
	virtual size_t GetEntityCount() = 0;
	virtual void SetEntityEnabled(const EntityID anEntityID, bool aEnabled) = 0;

	static EntityID Instantiate(PrefabID aPrefabID, SceneID aSceneID = -1);
	static EntityID Instantiate(const std::string& aPrefabName, SceneID aSceneID = -1);
};

template <class T>
T* SystemBase::GetSystem()
{
	if (!CSystemManager::instance)
		return nullptr;
	return CSystemManager::instance->GetSystem<T>();
}
