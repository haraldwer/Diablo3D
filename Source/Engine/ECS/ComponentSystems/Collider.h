#pragma once

#include "../Data/EntityData.h"
#include "../System.h"
#include "../../../../Include/PhysX/PxShape.h"

class ColliderData : public EntityData
{
	friend class Collider;
private:
	physx::PxShape* myShape;
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
