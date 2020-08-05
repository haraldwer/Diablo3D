#include "Rigidbody.h"
#include "../../Physics/PhysicsManager.h"

void Rigidbody::Init()
{
}

void Rigidbody::Update()
{
	
}

void Rigidbody::AddEntity(Entity* anEntity)
{
	if (!anEntity)
		return;
	auto& data = GetContainer().Insert(*anEntity);
	
	auto& physMan = ServiceLocator::Instance().GetService<PhysicsManager>();
	
	data.myActor = physMan.CreateDynamic(anEntity->GetTransform().GetPhysXRef(), data.myShape, anEntity->GetSceneID());
}

void Rigidbody::RemoveEntity(EntityID anEntityID)
{
	
}
