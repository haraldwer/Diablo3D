#pragma once
#include <string>
#include "CSystemManager.h"
#include <typeindex>
#include "../Entity/Prefab.h"
#include "Engine/Entity/Entity.h"
#include "RapidJSON/document.h"

class Entity;

class CSystemBase
{
	friend class CSystemManager;
	friend class PrefabManager;
	friend class Scene;
public:
	CSystemBase();
	virtual ~CSystemBase() = default;
	template <class T>
	static T* GetSystem();

private:
	// THESE ARE PURE VIRTUAL AND NEEDS TO BE IMPLEMENTED IN CHILD SYSTEMS
	virtual void Init() = 0;
	virtual void Update() = 0;
	virtual void AddEntity(Entity * anEntity) = 0;
	virtual void RemoveEntity(EntityID anEntityID) = 0;
	virtual void LoadPrefab(PrefabID, const rapidjson::GenericObject<false, rapidjson::Value>& aBase) = 0;
	virtual std::string GetSystemName() = 0;
	virtual std::type_index GetTypeIndex() = 0;
	virtual size_t GetEntityCount() = 0;
};

template <class T>
class CSystem : public CSystemBase
{
public:
	virtual ~CSystem() = default;
private:
	std::type_index GetTypeIndex() override;
};

template <class T>
T* CSystemBase::GetSystem()
{
	if (!CSystemManager::instance)
		return nullptr;
	return CSystemManager::instance->GetSystem<T>();
}

template <class T>
std::type_index CSystem<T>::GetTypeIndex()
{
	return std::type_index(typeid(T));
}

