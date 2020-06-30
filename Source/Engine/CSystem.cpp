#include "CSystem.h"

#include "CSystemManager.h"

CSystem::CSystem()
{
	CSystemManager::mySystems.push_back(this);
}

size_t CSystem::GetHash() const
{
	return myHash;
}
