#pragma once
#include <unordered_map>
#include <vector>
#include "CSystems/CModelRenderer.h"
#include <typeinfo>

class CSystemManager
{
	friend class CSystem;
public:
	void Init();
	void Update();
	template <class T>
	T* GetSystem();
	CSystem* GetSystem(const std::string& aSystemName);
private:
	static std::vector<CSystem*> mySystems;
	
	std::unordered_map<std::string, CSystem*> mySystemMap;
	std::unordered_map<size_t, CSystem*> mySystemHashMap;
	
	CModelRenderer myModelRenderer;
};

template <class T>
T* CSystemManager::GetSystem()
{
	const size_t hash = typeid(T).hash_code();
	return mySystemHashMap[hash];
}
