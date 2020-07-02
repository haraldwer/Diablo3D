#include "CSystemManager.h"
#include "CSystem.h"
#include "CSystems.h"

std::vector<CSystem*> CSystemManager::mySystems;
CSystemManager* CSystemManager::instance = nullptr;

CSystemManager::CSystemManager() : mySystemsClass(nullptr)
{
	instance = this;
}

CSystemManager::~CSystemManager()
{
	if (mySystemsClass)
	{
		delete(mySystemsClass);
		mySystemsClass = nullptr;
	}
	mySystems.clear();
	mySystemHashMap.clear();
	mySystemMap.clear();
}

void CSystemManager::Init()
{
	mySystemsClass = new CSystems();
	
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

CSystem* CSystemManager::GetSystem(const std::string& aSystemName)
{
	return mySystemMap[aSystemName];
}
