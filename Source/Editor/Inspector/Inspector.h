#pragma once
#include "Gizmo.h"
#include "../Engine/Engine.h"
#include "CommandQueue.h"


class Inspector
{
public:
	void Update(Gizmo& aGizmo, Engine* anEngine);
private:
	Entity* GetEntity(SceneID scene, EntityID entity);
	void EditTranform(Transform& aTransform);

	
	EntityID mySelectedID = 0;
	SceneID mySelectedScene = 3;
	bool myIsManipulating = false;

	Engine* myEngine;
	CommandQueue myCommandQueue;
};

