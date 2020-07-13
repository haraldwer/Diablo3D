#pragma once
#include "Gizmo.h"
#include "../Engine/Engine.h"
#include "CommandQueue.h"


class Inspector
{
public:
	void Update(Gizmo& aGizmo, Engine* anEngine);
	void SetEntity(const SceneID aScene, const EntityID aEntity);
	void EditTransformProperties(Transform& aTransform);
private:
	Entity* GetEntity(const SceneID aScene, const EntityID aEntity);
	void EditTranform(Transform& aTransform);

	
	EntityID mySelectedID = 0;
	SceneID mySelectedScene = 3;
	bool myIsManipulating = false;

	Engine* myEngine;
	CommandQueue myCommandQueue;
};

