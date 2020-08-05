#pragma once
#include "Engine/ECS/Data/EntityData.h"
#include "Engine/ECS/System.h"
#include <PhysX/PxActor.h>
#include <PhysX/geometry/PxBoxGeometry.h>

class RBData : public EntityData
{
	friend class Rigidbody;
public:
	RBData() :
	myIsStatic("myIsStatic", EditorControls::CHECKBOX)
	{}
	
private:
	Serializable<bool> myIsStatic;
	physx::PxActor* myActor;
	physx::PxBoxGeometry myShape;
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
