#include "ResourceBrowser.h"
#include "../Engine/Engine.h"
#include "../ImGui/imgui.h"
#include "ResourceViewer.h"

void ResourceBrowser::Init()
{
	for(int i = 0; i < ResourceType::_size(); i++)
	{
		ResourceType type = ResourceType::_from_index(i);
		ResourceViewer* viewer = CreateResourceViewer(type);
		if (viewer)
			myResourceViewers[type._to_index()] = viewer;
	}
}

void ResourceBrowser::Update(Engine* anEngine)
{
	if (!anEngine)
		return;

	ResourceManager& resourceManager = anEngine->GetServiceLocator().GetService<ResourceManager>();
	ResourceManager::Folder* base = resourceManager.GetFolder();
	ImGui::SetNextTreeNodeOpen(true);
	Folder(base);
	
	for (auto& it : myResourceViewers)
		it.second->WindowUpdate(anEngine);
}

void ResourceBrowser::Folder(ResourceManager::Folder* aFolder)
{
	if (!aFolder)
		return;
	
	if(ImGui::TreeNodeEx(aFolder->name.c_str()))
	{
		for(auto& it : aFolder->folders)
			Folder(it);

		for(auto& it : aFolder->resources)
		{
			if (ImGui::TreeNodeEx(it->myName.c_str(), ImGuiTreeNodeFlags_Leaf))
				ImGui::TreePop();
			if (ImGui::IsItemClicked())
				SelectResource(it);
		}

		for(auto& it : aFolder->unloadedResources)
		{
			if (ImGui::TreeNodeEx((it->name + " (unloaded)").c_str(), ImGuiTreeNodeFlags_Leaf))
				ImGui::TreePop();
			//if (ImGui::IsItemClicked())
		}
		ImGui::TreePop();
	}
}

void ResourceBrowser::SelectResource(EngineResource* aResource)
{
	if(!aResource)
	{
		Debug::Error << "Resource ptr was null" << std::endl;
		return;
	}
	Debug::Log << "Resource with name " << aResource->myName << " selected" << std::endl;
	auto ptr = myResourceViewers.find(aResource->myType._to_index());
	if(ptr != myResourceViewers.end())
		if(ptr->second)
			ptr->second->Select(aResource->myID);
}

ResourceViewer* ResourceBrowser::CreateResourceViewer(const ResourceType& aType)
{
	ResourceViewer* viewer;
	switch(aType)
	{
	case ResourceType::MODEL:
		break;
	case ResourceType::TEXTURE:
		break;
	case ResourceType::PREFAB:
		break;
	case ResourceType::SCENE:
		break;
	default:
		return nullptr;
	}
	viewer = new ResourceViewer(aType);
	return viewer;
}