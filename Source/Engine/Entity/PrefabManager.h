#pragma once
#include "Prefab.h"
#include "../Utility/ServiceLocator.h"

class EngineResource;

class PrefabManager : Service<PrefabManager>
{
public:
	void Init();
	std::string GetName() override { return "Prefab Manager"; }
	void Editor() override;
	Prefab* GetPrefab(PrefabID aPrefabID);
	PrefabID GetPrefabID(const std::string& aName);
	void Save();
private:
	void Load();
	void LoadPrefab(EngineResource* resource);
	std::unordered_map<PrefabID, Prefab*> myPrefabs;
	std::unordered_map<std::string, PrefabID> myPrefabNames;
};

