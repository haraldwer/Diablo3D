#pragma once
#include "../Utility/ServiceLocator.h"
#include "../../Include/PhysX/PxPhysicsAPI.h"
#include "../Scenes/Scene.h"
#include "../Engine/ECS/ComponentSystems/Collider.h"

#define PVD_HOST "127.0.0.1"

class PhysicsManager : Service<PhysicsManager>
{
public:
	PhysicsManager();
	std::string GetName() override { return "Physics Manager"; }
	void Init();
	void Update(bool run);
	void Shutdown();
	
	void				CreateScene(SceneID aSceneID);
	void				DestroyScene(SceneID aSceneID);
	
	physx::PxShape*		CreateShape(const physx::PxGeometry& aGeometry);
	physx::PxShape*		CreateShape(const physx::PxGeometry& aGeometry, const physx::PxMaterial& aMaterial);
	physx::PxRigidBody*	CreateRigidDynamic(const physx::PxTransform& aPxTransform, SceneID aScene);


private:
	physx::PxDefaultAllocator		myAllocator;
	physx::PxDefaultErrorCallback	myErrorCallback;
	physx::PxFoundation*			myFoundation;
	physx::PxPhysics*				myPhysics;
	physx::PxDefaultCpuDispatcher*	myDispatcher;
	physx::PxMaterial*				myMaterial;
	physx::PxPvd*					myPvd;

	std::unordered_map<SceneID, physx::PxScene*> myScenes; // One physics scene for each actual scene
};

