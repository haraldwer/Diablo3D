#include "Entity.h"
#include "../Utility/JSONHelper.h"
#include "Engine/Utility/ServiceLocator.h"
#include "Engine/Scenes/SceneManager.h"
#include "Engine/ECS/CSystemManager.h"
#include "Engine/ECS/SystemBase.h"

EntityID Entity::GetID() const
{
	return myID;
}

SceneID Entity::GetSceneID() const
{
	return mySceneID;
}

PrefabID Entity::GetPrefabID() const
{
	return myPrefabID;
}

Transform& Entity::GetTransform()
{
	return myTransform;
}

void Entity::Serialize(rapidjson::Writer<rapidjson::StringBuffer>& aBase) const
{
	aBase.StartObject();
	Serialize::Serialize("Prefab", myPrefabID, aBase);
	Serialize::Serialize("Transform", myTransform.GetMatrix(), aBase);
	auto& sysMan = ServiceLocator::Instance().GetService<CSystemManager>();
	aBase.Key("Overrides");
	aBase.StartObject();
	for (auto& it : mySystemRefs)
	{
		auto sys = sysMan.GetSystem(it);
		if (sys)
			sys->GetEntityOverrides(myID, aBase);
	}
	aBase.EndObject();
	aBase.EndObject();
}

void Entity::Deserialize(const rapidjson::Value::Object& aContainer)
{
	CommonUtilities::Matrix4x4<float> matrix = myTransform.GetMatrix();
	Deserialize::Deserialize(aContainer, "Transform", matrix);
	myTransform.SetMatrix(matrix);
	if(aContainer.HasMember("Overrides") && aContainer["Overrides"].IsObject())
	{
		auto& sysMan = ServiceLocator::Instance().GetService<CSystemManager>();
		for(auto& it : mySystemRefs)
		{
			auto sys = sysMan.GetSystem(it);
			if(sys)
				sys->ApplyEntityOverrides(myID, aContainer["Overrides"].GetObject());
		}
	}
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

void Entity::SetEnabled(bool aEnabled)
{
	CSystemManager& sysMan = ServiceLocator::Instance().GetService<CSystemManager>();
	sysMan.SetEntityEnabled(myID, mySystemRefs, aEnabled);
	myEnabled = aEnabled;
}

bool Entity::GetEnabled() const
{
	return myEnabled;
}

std::vector<std::type_index> Entity::GetSystemRefs() const
{
	return mySystemRefs;
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
