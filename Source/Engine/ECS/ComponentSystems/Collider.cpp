#include "Collider.h"
#include "../../Physics/PhysicsManager.h"
#include "Rigidbody.h"

void Collider::Init()
{
}

void Collider::Update()
{
}

void Collider::AddEntity(Entity* anEntity)
{
	if (!anEntity)
		return;
	auto& data = GetContainer().Insert(*anEntity);

	auto& physMan = ServiceLocator::Instance().GetService<PhysicsManager>();
	data.myShape = physMan.CreateShape(physx::PxBoxGeometry(0.5f, 0.5f, 0.5f));
	if (!data.myShape)
		return;
	
	auto rb = GetSystem<Rigidbody>();
	if(!rb)
		return;
	if(!rb->GetContainer().HasData(anEntity->GetID()))
		return;
	
	auto& rbData = rb->GetContainer().GetData(anEntity->GetID());
	rbData.myBody->attachShape(*data.myShape);
}

void Collider::RemoveEntity(EntityID anEntityID)
{
	auto& data = GetContainer().GetData(anEntityID);
	
	auto rb = GetSystem<Rigidbody>();
	if (!rb)
		return;
	if (!rb->GetContainer().HasData(anEntityID))
		return;

	auto& rbData = rb->GetContainer().GetData(anEntityID);
	rbData.myBody->detachShape(*data.myShape);
	data.myShape->release();
	
	GetContainer().Remove(anEntityID);
}
