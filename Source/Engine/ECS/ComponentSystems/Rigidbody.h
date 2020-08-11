#pragma once
#include "Engine/ECS/Data/EntityData.h"
#include "Engine/ECS/System.h"
#include <PhysX/PxRigidBody.h>

class RBData : public EntityData
{
	friend class Rigidbody;
	friend class Collider;
public:
	RBData() : myBody(nullptr)
	{}
	
private:
	physx::PxRigidBody* myBody;
};

class Rigidbody : public System<Rigidbody, RBData>
{
public:
	std::string GetName() override { return "Rigidbody"; };
	void Init() override;
	void Update() override;
	void AddEntity(Entity* anEntity) override;
	void RemoveEntity(EntityID anEntityID) override;
};
