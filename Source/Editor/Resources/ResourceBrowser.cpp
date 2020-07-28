#include "ResourceBrowser.h"
#include "../Engine/Engine.h"
#include "../ImGui/imgui.h"
#include "ResourceViewer.h"
#include "PrefabViewer.h"

ResourceBrowser::ResourceBrowser(): myManager(nullptr)
{
}

void ResourceBrowser::Update(Engine* anEngine)
{
	if (!anEngine)
	{
		myManager = nullptr;
		return;
	}

	myManager = &anEngine->GetServiceLocator().GetService<ResourceManager>();
	ResourceManager::Folder* base = myManager->GetFolder();
	ImGui::SetNextTreeNodeOpen(true);
	Folder(base);

	for (int i = myResourceViewers.size() - 1; i >= 0; i--)
		if (!myResourceViewers[i]->WindowUpdate(anEngine))
			myResourceViewers.erase(myResourceViewers.begin() + i);
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
			if(!it)
			{
				if (ImGui::TreeNodeEx("Unable to get resource ptr", ImGuiTreeNodeFlags_Leaf))
					ImGui::TreePop();
				continue;
			}

			ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_Leaf;
			const bool selected = GetIsResourceSelected(it->myID);
			if (selected)
				flags = ImGuiTreeNodeFlags_Bullet;
			const bool open = ImGui::TreeNodeEx(it->myName.c_str(), flags);
			if (ImGui::BeginPopupContextItem())
			{
				if(LoadedContextMenu(it))
				{
					ImGui::EndPopup();
					if (open)
						ImGui::TreePop();
					ImGui::TreePop();
					return;
				}
				ImGui::EndPopup();
			}
			if (open)
				ImGui::TreePop();
			if (ImGui::IsItemClicked())
				selected ? DeselectResource(it) : SelectResource(it); // If already selected, deselect. Else select.
		}

		for(auto& it : aFolder->unloadedResources)
		{
			const bool open = ImGui::TreeNodeEx((it->name + " (unloaded)").c_str(), ImGuiTreeNodeFlags_Leaf);
			if (ImGui::BeginPopupContextItem())
			{
				if(UnloadedContextMenu(it))
				{
					ImGui::EndPopup();
					if (open)
						ImGui::TreePop();
					ImGui::TreePop();
					return;
				}
				ImGui::EndPopup();
			}
			if (open)
				ImGui::TreePop();
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
	ResourceViewer* resourceViewer = CreateResourceViewer(aResource);
	if(!resourceViewer)
	{
		Debug::Error << "Unable to create ResourceViewer" << std::endl;
		return;
	}
	myResourceViewers.push_back(resourceViewer);
}

void ResourceBrowser::DeselectResource(EngineResource* aResource)
{
	for(int i = 0; i < myResourceViewers.size(); i++)
	{
		if (myResourceViewers[i]->GetResourceID() == aResource->myID)
		{
			myResourceViewers.erase(myResourceViewers.begin() + i);
			return;
		}
	}
}

bool ResourceBrowser::GetIsResourceSelected(ResourceID anID)
{
	for (auto& it : myResourceViewers)
		if (it->GetResourceID() == anID)
			return true;
	return false;
}

ResourceViewer* ResourceBrowser::CreateResourceViewer(EngineResource* aResource)
{
	if (!aResource)
		return nullptr;
	ResourceViewer* viewer = nullptr;
	switch(aResource->myType)
	{
	case ResourceType::PREFAB:
		viewer = new PrefabViewer(aResource->myType, aResource->myID);
		break;
	case ResourceType::MODEL:
	case ResourceType::TEXTURE:
	case ResourceType::SCENE:
	default:
		viewer = new ResourceViewer(aResource->myType, aResource->myID);
		break;
	}
	return viewer;
}

bool ResourceBrowser::LoadedContextMenu(EngineResource* aResource)
{
	if (!aResource || !myManager)
		return false;
	
	if(ImGui::MenuItem("Rename"))
	{
	}
	if(ImGui::MenuItem("Unload"))
	{
		myManager->UnloadResource(aResource->myID);
		return true;
	}
	if(ImGui::MenuItem("Delete"))
	{
		myManager->DeleteResource(aResource->myID);
		return true;
	}
	return false;
}

bool ResourceBrowser::UnloadedContextMenu(ResourceManager::UnloadedResource* aResource)
{
	if (!aResource || !myManager)
		return false;
	if(ImGui::MenuItem("Import"))
	{
		myManager->ImportResource(aResource->path);
		return true;
	}
	if(ImGui::MenuItem("Delete"))
	{
		myManager->DeleteUnloadedResource(aResource->path);
		return true;
	}
	return false;
}

