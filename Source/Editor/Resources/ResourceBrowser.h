#pragma once
#include "../../Engine/EngineResources/ResourceManager.h"
class Engine;
class ResourceViewer;

class ResourceBrowser
{
public:
	ResourceBrowser();
	void Update(Engine* anEngine);
	void Folder(ResourceManager::Folder* aFolder);
	void SelectResource(EngineResource* aResource);
	void DeselectResource(EngineResource* aResource);
	bool GetIsResourceSelected(ResourceID anID);
	ResourceViewer* CreateResourceViewer(EngineResource* aResource);
private:
	bool LoadedContextMenu(EngineResource* aResource);
	bool UnloadedContextMenu(ResourceManager::UnloadedResource* aResource);
	std::vector<ResourceViewer*> myResourceViewers;
	ResourceManager* myManager;
};
