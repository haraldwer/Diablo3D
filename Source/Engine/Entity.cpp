#include "Entity.h"

EntityID Entity::GetID() const
{
	return myID;
}

void Entity::Construct(const EntityID anID)
{
	myID = anID;
}

void Entity::Destruct()
{
	myID = -1;
}
