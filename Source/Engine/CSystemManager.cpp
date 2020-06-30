#include "CSystemManager.h"
#include "CSystem.h"

std::vector<CSystem*> CSystemManager::mySystems;

void CSystemManager::Init()
{
	for (auto& it : mySystems)
	{
		mySystemMap[it->GetSystemName()] = it;
		mySystemHashMap[it->GetHash()] = it;
	}
	
	for (auto& it : mySystems)
		it->Init();
}

void CSystemManager::Update()
{
	for (auto& it : mySystems)
		it->Update();
}

void CSystemManager::Edit()
{
	
}

CSystem* CSystemManager::GetSystem(const std::string& aSystemName)
{
	return mySystemMap[aSystemName];
}
