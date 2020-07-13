#pragma once
#include "../../Engine/EngineResources/ResourceManager.h"
class Engine;
class ResourceViewer;

class ResourceBrowser
{
public:
	void Init();
	void Update(Engine* anEngine);
	void Folder(ResourceManager::Folder* aFolder);
	void SelectResource(EngineResource* aResource);
	ResourceViewer* CreateResourceViewer(const ResourceType& aType);
private:
	std::unordered_map<int, ResourceViewer*> myResourceViewers;
};

