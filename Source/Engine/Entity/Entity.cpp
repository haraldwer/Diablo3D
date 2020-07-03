#include "Entity.h"

EntityID Entity::GetID() const
{
	return myID;
}

PrefabID Entity::GetPrefabID() const
{
	return myPrefabID;
}

void Entity::Construct(const EntityID anID, const PrefabID aPrefabID)
{
	myID = anID;
	myPrefabID = aPrefabID;
}

void Entity::Destruct()
{
	myID = -1;
	myPrefabID = -1;
}
