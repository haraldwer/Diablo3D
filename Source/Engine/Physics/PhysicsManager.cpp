#include "PhysicsManager.h"

PhysicsManager::PhysicsManager(): myFoundation(nullptr), myPhysics(nullptr), myDispatcher(nullptr),
                                  myMaterial(nullptr),
                                  myPvd(nullptr)
{
}

void PhysicsManager::Init()
{
	myFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, myAllocator, myErrorCallback);

	myPvd = PxCreatePvd(*myFoundation);
	physx::PxPvdTransport* transport = physx::PxDefaultPvdSocketTransportCreate(PVD_HOST, 5425, 10);
	myPvd->connect(*transport, physx::PxPvdInstrumentationFlag::eALL);

	myPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *myFoundation, physx::PxTolerancesScale(), true, myPvd);
	myMaterial = myPhysics->createMaterial(0.5f, 0.5f, 0.6f);

	//physx::PxRigidStatic* groundPlane = PxCreatePlane(*myPhysics, physx::PxPlane(0, 1, 0, 0), *myMaterial);
	//myScene->addActor(*groundPlane);
}

void PhysicsManager::Update()
{
	for(auto& scene : myScenes)
	{
		scene.second->simulate(1.0f / 60.0f);
		scene.second->fetchResults(true);
	}
}

void PhysicsManager::Shutdown()
{
	for(auto& it : myScenes)
	{
		if(it.second)
		{
			it.second->release();
			it.second = nullptr;
		}
	}
	myScenes.clear();
	if(myDispatcher)
	{
		myDispatcher->release();
		myDispatcher = nullptr;
	}
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

physx::PxRigidDynamic* PhysicsManager::CreateDynamic(const physx::PxTransform& t, const physx::PxGeometry& geometry, SceneID scene)
{
	if(!myScenes[scene])
	{
		Debug::Error << "Unable to create dynamic, PhysicsScene with ID " << scene << " does not exist" << std::endl;
		return nullptr;
	}
	physx::PxRigidDynamic* dynamic = PxCreateDynamic(*myPhysics, t, geometry, *myMaterial, 10.0f);
	dynamic->setAngularDamping(0.5f);
	myScenes[scene]->addActor(*dynamic);
	return dynamic;
}

physx::PxRigidStatic* PhysicsManager::CreateStatic(const physx::PxTransform& t, const physx::PxGeometry& geometry, SceneID scene)
{
	if (!myScenes[scene])
	{
		Debug::Error << "Unable to create dynamic, PhysicsScene with ID " << scene << " does not exist" << std::endl;
		return nullptr;
	}
	physx::PxRigidStatic* rb = physx::PxCreateStatic(*myPhysics, t, geometry, *myMaterial);
	myScenes[scene]->addActor(*rb);
	return rb;
}

void PhysicsManager::DestroyRigidbody(physx::PxActor* rb, SceneID scene)
{
	if (!rb)
		return;
	if (!myScenes[scene])
	{
		Debug::Error << "Cannot remove dynamic from scene which does not exist. SceneID: " << scene << std::endl;
	}
	else
	{
		myScenes[scene]->removeActor(*rb);
	}
	rb->release();
}

void PhysicsManager::CreateScene(SceneID aSceneID)
{
	if (myScenes.find(aSceneID) != myScenes.end() && myScenes[aSceneID] != nullptr)
	{
		Debug::Warning << "PhysicsScene with ID " << aSceneID << " already exists. Recreating..." << std::endl;
		DestroyScene(aSceneID);
	}
	
	physx::PxSceneDesc sceneDesc(myPhysics->getTolerancesScale());
	sceneDesc.gravity = physx::PxVec3(0.0f, -9.81f, 0.0f);
	myDispatcher = physx::PxDefaultCpuDispatcherCreate(2);
	sceneDesc.cpuDispatcher = myDispatcher;
	sceneDesc.filterShader = physx::PxDefaultSimulationFilterShader;
	
	auto scene = myPhysics->createScene(sceneDesc);

	physx::PxPvdSceneClient* pvdClient = scene->getScenePvdClient();
	if (pvdClient) // Print debug info
	{
		pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
		pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
		pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
	}
}

void PhysicsManager::DestroyScene(SceneID aSceneID)
{
	if(myScenes.find(aSceneID) == myScenes.end())
	{
		Debug::Warning << "Trying to destroy a PhysicsScene which does not exist. SceneID: " << aSceneID << std::endl;
		return;
	}
	if(myScenes[aSceneID] == nullptr)
	{
		Debug::Warning << "Trying to destroy a PhysicsScene which has already been destroyed. SceneID: " << aSceneID << std::endl;
		return;
	}
	myScenes[aSceneID]->release();
	myScenes[aSceneID] = nullptr;
	myScenes.erase(aSceneID);
}
