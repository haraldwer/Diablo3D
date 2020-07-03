#pragma once
#include <unordered_map>
#include "EngineResource.h"
#include "../Utility/ServiceLocator.h"

class ResourceManager : public Service<ResourceManager>
{
public:
	void LoadResources(const std::string& aPath);
	EngineResource* CreateResource(const std::string& aResourceName, ResourceType aType);
	EngineResource* GetResource(ResourceID anID);
	std::vector<EngineResource*> GetResources(ResourceType aType);
	std::string GetName() override { return "Resource Manager"; }
	void Editor() override;
private:
	void LoadResourcesRec(const std::string& aPath);
	void LoadResource(const std::string& aPath);
	std::unordered_map<ResourceID, EngineResource*> myResources;
	std::unordered_map<int, std::vector<EngineResource*>> myResourceTypeMap;
};
