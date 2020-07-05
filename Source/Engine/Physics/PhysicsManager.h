#pragma once
#include "../Utility/ServiceLocator.h"
#include "../../Include/PhysX/PxPhysicsAPI.h"

#define PVD_HOST "127.0.0.1"

class PhysicsManager : Service<PhysicsManager>
{
public:
	PhysicsManager();
	std::string GetName() override { return "Physics Manager"; }
	void Init();
	void Update();
	void Shutdown();
	
private:
	physx::PxRigidDynamic* CreateDynamic(const physx::PxTransform& t, const physx::PxGeometry& geometry);
	
	physx::PxDefaultAllocator		myAllocator;
	physx::PxDefaultErrorCallback	myErrorCallback;
	physx::PxFoundation*			myFoundation;
	physx::PxPhysics*				myPhysics;
	physx::PxDefaultCpuDispatcher*	myDispatcher;
	physx::PxScene*					myScene;
	physx::PxMaterial*				myMaterial;
	physx::PxPvd*					myPvd;
};

