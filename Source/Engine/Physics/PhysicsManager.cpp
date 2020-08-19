#include "PhysicsManager.h"
#include "Engine/Utility/Time.h"

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

void PhysicsManager::Update(bool run)
{
	if (!run)
		return;
	for(auto& scene : myScenes)
	{
		if (!scene.second)
			continue;
		scene.second->simulate(Time::Delta());
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

physx::PxShape* PhysicsManager::CreateShape(const physx::PxGeometry& aGeometry)
{
	return CreateShape(aGeometry, *myMaterial);
}

physx::PxShape* PhysicsManager::CreateShape(const physx::PxGeometry& aGeometry, const physx::PxMaterial& aMaterial)
{
	return myPhysics->createShape(aGeometry, aMaterial);
}

physx::PxRigidBody* PhysicsManager::CreateRigidDynamic(const physx::PxTransform& aPxTransform, SceneID aScene)
{
	auto find = myScenes.find(aScene);
	if (find == myScenes.end())
		return nullptr;
	auto ptr = myPhysics->createRigidDynamic(aPxTransform);
	if(ptr)
		find->second->addActor(*ptr);
	return ptr;
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
	myScenes[aSceneID] = scene;
	
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
