#include "PhysicsManager.h"

PhysicsManager::PhysicsManager(): myFoundation(nullptr), myPhysics(nullptr), myDispatcher(nullptr), myScene(nullptr),
                                  myMaterial(nullptr),
                                  myPvd(nullptr)
{
}

void PhysicsManager::Init()
{
	myFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, myAllocator, myErrorCallback);

	myPvd = physx::PxCreatePvd(*myFoundation);
	physx::PxPvdTransport* transport = physx::PxDefaultPvdSocketTransportCreate(PVD_HOST, 5425, 10);
	myPvd->connect(*transport, physx::PxPvdInstrumentationFlag::eALL);

	myPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *myFoundation, physx::PxTolerancesScale(), true, myPvd);

	physx::PxSceneDesc sceneDesc(myPhysics->getTolerancesScale());
	sceneDesc.gravity = physx::PxVec3(0.0f, -9.81f, 0.0f);
	myDispatcher = physx::PxDefaultCpuDispatcherCreate(2);
	sceneDesc.cpuDispatcher = myDispatcher;
	sceneDesc.filterShader = physx::PxDefaultSimulationFilterShader;
	myScene = myPhysics->createScene(sceneDesc);

	physx::PxPvdSceneClient* pvdClient = myScene->getScenePvdClient();
	if (pvdClient)
	{
		pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
		pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
		pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
	}
	myMaterial = myPhysics->createMaterial(0.5f, 0.5f, 0.6f);

	physx::PxRigidStatic* groundPlane = PxCreatePlane(*myPhysics, physx::PxPlane(0, 1, 0, 0), *myMaterial);
	myScene->addActor(*groundPlane);
}

void PhysicsManager::Update()
{
	myScene->simulate(1.0f / 60.0f);
	myScene->fetchResults(true);
}

void PhysicsManager::Shutdown()
{
	myScene->release();
	myScene = nullptr;
	myDispatcher->release();
	myDispatcher = nullptr;
	myPhysics->release();
	myPhysics = nullptr;
	if (myPvd)
	{
		physx::PxPvdTransport* transport = myPvd->getTransport();
		myPvd->release();
		myPvd = nullptr;
		transport->release();
		transport = nullptr;
	}
	myFoundation->release();
	myFoundation = nullptr;
}

physx::PxRigidDynamic* PhysicsManager::CreateDynamic(const physx::PxTransform& t, const physx::PxGeometry& geometry)
{
	physx::PxRigidDynamic* dynamic = PxCreateDynamic(*myPhysics, t, geometry, *myMaterial, 10.0f);
	dynamic->setAngularDamping(0.5f);
	myScene->addActor(*dynamic);
	return dynamic;
}
