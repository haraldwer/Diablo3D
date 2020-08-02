#pragma once
#include <unordered_map>
#include <vector>
#include <typeindex>
#include "../Utility/ServiceLocator.h"
#include "../Entity/PrefabManager.h"

class SystemBase;
class CSystems;
class EngineResource;

class CSystemManager : public Service<CSystemManager>
{
	friend class SystemBase;
public:
	CSystemManager();
	~CSystemManager();
	void Init();
	void Update(bool run);
	SystemBase* GetSystem(const std::string& aSystemName);
	SystemBase* GetSystem(const std::type_index& aSystemTypeIndex);
	template <class T>
	T* GetSystem();
	std::string GetName() override { return "CS Manager"; }
	void Editor() override;
	void RemoveEntity(const EntityID anEntityID, const std::vector<std::type_index>& someSystemTypes);
	void SetEntityEnabled(const EntityID anEntityID, const std::vector<std::type_index>& someSystemTypes, bool aEnabled);
	std::vector<std::string> GetSystems();
private:
	static std::vector<SystemBase*> mySystems;
	static CSystemManager* instance;
	std::unordered_map<std::string, SystemBase*> mySystemMap;
	std::unordered_map<std::type_index, SystemBase*> mySystemHashMap;
	CSystems* mySystemsClass;
	PrefabManager myPrefabManager;
};

template <class T>
T* CSystemManager::GetSystem()
{
	const auto hash = std::type_index(typeid(T));
	auto find = mySystemHashMap.find(hash);
	if (find == mySystemHashMap.end())
		return nullptr;
	return reinterpret_cast<T*>(find->second);
}
