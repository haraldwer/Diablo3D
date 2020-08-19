#include "Rigidbody.h"
#include "../../Physics/PhysicsManager.h"

void Rigidbody::Init()
{
}

void Rigidbody::Update()
{
	GetContainer().Iterate([&](const EntityID anEntityID, RBData& someData)
	{
		someData.GetEntity()->GetTransform().UpdatePhysRef();
		return true;
	});
}

void Rigidbody::AddEntity(Entity* anEntity)
{
	if (!anEntity)
		return;
	auto& trans = anEntity->GetTransform().GetPhysXRef();
	auto& data = GetContainer().Insert(*anEntity);
	auto& physMan = ServiceLocator::Instance().GetService<PhysicsManager>();	
	data.myBody = physMan.CreateRigidDynamic(trans, anEntity->GetSceneID());
}

void Rigidbody::RemoveEntity(EntityID anEntityID)
{
	auto& data = GetContainer().GetData(anEntityID);
	data.myBody->release();
	GetContainer().Remove(anEntityID);
}
