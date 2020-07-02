#pragma once
#include <string>
#include "CSystemManager.h"

class CSystem
{
public:
	
	// THESE ARE PURE VIRTUAL AND NEEDS TO BE IMPLEMENTED IN CHILD SYSTEMS
	virtual void Init() = 0;
	virtual void Update() = 0;
	virtual std::string GetSystemName() = 0;
	
	CSystem();
	virtual ~CSystem() = default;
	template <class T>
	static T* GetSystem();
	size_t GetHash() const;

private:
	size_t myHash;
};

template <class T>
T* CSystem::GetSystem()
{
	if (!CSystemManager::instance)
		return nullptr;
	return CSystemManager::instance->GetSystem<T>();
}

