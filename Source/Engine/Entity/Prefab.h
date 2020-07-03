#pragma once
#include <vector>
#include "../EngineResources/EngineResource.h"

typedef ResourceID PrefabID;

class Prefab
{
	friend class PrefabManager;
public:
	std::vector<std::string> GetComponents() const;
	std::string GetName() const;
private:
	std::vector<std::string> myComponents;
	PrefabID myID;
	std::string myName;
	// Should this contain ref to instatiated versions of self? 
};