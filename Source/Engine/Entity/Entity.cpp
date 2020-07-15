#include "Entity.h"
#include "../Utility/JSONHelper.h"

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

void Entity::Construct(const EntityID anID, const PrefabID aPrefabID)
{
	myID = anID;
	myPrefabID = aPrefabID;
	myTransform = Transform();
}

void Entity::Destruct()
{
	myID = -1;
	myPrefabID = -1;
}
