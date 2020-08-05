#pragma once
#include "../../Engine/EngineResources/ResourceManager.h"
class Engine;
class ResourceViewer;

class ResourceBrowser
{
public:
	ResourceBrowser();
	void Update(Engine* anEngine);
	void SelectResource(EngineResource* aResource);
	void DeselectResource(EngineResource* aResource);
	bool GetIsResourceSelected(ResourceID anID);
	ResourceViewer* CreateResourceViewer(EngineResource* aResource);
private:
	void Folder(ResourceManager::Folder* aFolder);
	void File(EngineResource* aResource, ResourceManager::Folder* aFolder);
	bool CopyLocal(const std::string& aSrc, const std::string& aDest);
	bool CopyDrop(const std::string& aDest);
	bool NormalDrop(const std::string& aDest);
	bool LoadedContextMenu(EngineResource* aResource);
	bool UnloadedContextMenu(ResourceManager::UnloadedResource* aResource);
	std::vector<ResourceViewer*> myResourceViewers;
	ResourceManager* myManager;
};
