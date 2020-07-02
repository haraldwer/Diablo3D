#pragma once
#include <unordered_map>
#include <vector>
#include <typeinfo>

class CSystem;
class CSystems;

class CSystemManager
{
	friend class CSystem;
public:
	CSystemManager();
	~CSystemManager();
	void Init();
	void Update();
	CSystem* GetSystem(const std::string& aSystemName);
private:
	template <class T>
	T* GetSystem();
	static std::vector<CSystem*> mySystems;
	static CSystemManager* instance;
	std::unordered_map<std::string, CSystem*> mySystemMap;
	std::unordered_map<size_t, CSystem*> mySystemHashMap;
	CSystems* mySystemsClass;
};

template <class T>
T* CSystemManager::GetSystem()
{
	const size_t hash = typeid(T).hash_code();
	auto find = mySystemHashMap.find(hash);
	if (find == mySystemHashMap.end())
		return nullptr;
	return reinterpret_cast<T*>(find->second);
}
