#pragma once
#include "Gizmo.h"
#include "../Engine/Engine.h"
#include "CommandQueue.h"
#include "../Preferences.h"

class SerializableBase;

class Inspector
{
public:
	Inspector();
	void Update(Gizmo& aGizmo, Engine* anEngine);
	void SetEntity(const SceneID aScene, const EntityID aEntity);
	bool GetIsSelected(SceneID aSceneID, EntityID aEntityID);
private:
	Entity* GetEntity(const SceneID aScene, const EntityID aEntity);
	void EditTranform(Transform& aTransform);
	void EditComponents(Entity* anEntity);
	void Duplicate();
	void Save();
	
	Preference<EntityID> mySelectedID;
	Preference<SceneID> mySelectedScene;
	bool myIsManipulating = false;

	Engine* myEngine;
	CommandQueue myCommandQueue;

	std::unordered_map<std::type_index, std::function<void(SerializableBase* aBase)>> mySerializableFunctions;
};