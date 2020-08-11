#include "Rigidbody.h"
#include "../../Physics/PhysicsManager.h"

void Rigidbody::Init()
{
}

void Rigidbody::Update()
{
	GetContainer().Iterate([&](const EntityID anEntityID, RBData& someData)
	{
		Transform& trans = someData.GetEntity()->GetTransform();
		const auto p = trans.GetPhysXRef().p;
		trans.SetPosition({ p.x, p.y, p.z });
		return true;
	});
}

void Rigidbody::AddEntity(Entity* anEntity)
{
	if (!anEntity)
		return;
	auto& data = GetContainer().Insert(*anEntity);
	auto& physMan = ServiceLocator::Instance().GetService<PhysicsManager>();	
	data.myBody = physMan.CreateRigidDynamic(anEntity->GetTransform().GetPhysXRef(), anEntity->GetSceneID());
}

void Rigidbody::RemoveEntity(EntityID anEntityID)
{
	auto& data = GetContainer().GetData(anEntityID);
	data.myBody->release();
	GetContainer().Remove(anEntityID);
}
