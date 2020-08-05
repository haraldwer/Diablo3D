#pragma once
#include "Engine/ECS/Data/EntityData.h"
#include "Engine/ECS/System.h"

class ColliderData : public EntityData
{
};

class Collider : public System<Collider, ColliderData>
{
public:
	std::string GetName() override { return "Collider"; };
	void Init() override;
	void Update() override;
	void AddEntity(Entity* anEntity) override;
	void RemoveEntity(EntityID anEntityID) override;
};
