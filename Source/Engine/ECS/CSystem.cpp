#include "CSystem.h"
#include "CSystemManager.h"

CSystemBase::CSystemBase()
{
	CSystemManager::mySystems.push_back(this);
}