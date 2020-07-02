#include "CSystem.h"

#include "CSystemManager.h"

CSystem::CSystem() : myHash(0)
{
	CSystemManager::mySystems.push_back(this);
}

size_t CSystem::GetHash() const
{
	return myHash;
}
