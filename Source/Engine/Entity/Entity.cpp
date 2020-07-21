#include "Entity.h"
#include "../Utility/JSONHelper.h"
#include "Engine/Utility/ServiceLocator.h"
#include "Engine/Scenes/SceneManager.h"
#include "Engine/ECS/CSystemManager.h"
#include "Engine/ECS/CSystem.h"

EntityID Entity::GetID() const
{
	return myID;
}

PrefabID Entity::GetPrefabID() const
{
	return myPrefabID;
}

Transform& Entity::GetTransform()
{
	return myTransform;
}

void Entity::Save(rapidjson::Writer<rapidjson::StringBuffer>& aBase) const
{
	aBase.StartObject();
	Serialize::Serialize("Prefab", myPrefabID, aBase);
	Serialize::Serialize("Transform", myTransform.GetMatrix(), aBase);
	aBase.EndObject();
}

bool Entity::Destroy()
{
	Scene* scene = ServiceLocator::Instance().GetService<SceneManager>().GetScene(mySceneID);
	if(!scene)
	{
		Debug::Error << "Unable to destroy entity with ID " << myID << ", could not find scene with ID" << mySceneID << std::endl;
		return false;
	}
	return scene->DestroyEntity(myID);
}

void Entity::AddSystem(const std::type_index& aTypeIndex)
{
	mySystemRefs.push_back(aTypeIndex);
}

void Entity::Construct(const EntityID anID, const PrefabID aPrefabID, const SceneID aSceneID)
{
	myID = anID;
	myPrefabID = aPrefabID;
	mySceneID = aSceneID;
	myTransform = Transform();
	mySystemRefs.clear();
}

void Entity::Destruct()
{
	myID = -1;
	myPrefabID = -1;
	mySceneID = -1;
	myTransform = Transform();
	CSystemManager& sysMan = ServiceLocator::Instance().GetService<CSystemManager>();
	sysMan.RemoveEntity(myID, mySystemRefs);
	mySystemRefs.clear();
}
