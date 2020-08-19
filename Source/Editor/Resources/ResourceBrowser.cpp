#include "ResourceBrowser.h"
#include "../Engine/Engine.h"
#include "../ImGui/imgui.h"
#include "ResourceViewer.h"
#include "PrefabViewer.h"
#include "../DropManager.h"
#include <fstream>
#include "SceneViewer.h"

ResourceBrowser::ResourceBrowser(): myManager(nullptr), myJustOpened(false)
{
}

void ResourceBrowser::Update(Engine* anEngine)
{
	if(myJustOpened)
	{
		myJustOpened = false;
		ImGui::SetWindowFocus();
	}
	
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
	{
		if (!myResourceViewers[i]->WindowUpdate(anEngine))
		{
			delete(myResourceViewers[i]);
			myResourceViewers.erase(myResourceViewers.begin() + i);
		}
	}
}

void ResourceBrowser::Folder(ResourceManager::Folder* aFolder)
{
	if (!aFolder)
		return;
	
	if(ImGui::TreeNodeEx(aFolder->name.c_str()))
	{
		if (ImGui::IsItemHovered())
			CopyDrop(aFolder->path);
		NormalDrop(aFolder->path);
		
		for(auto& it : aFolder->folders)
			Folder(it);

		for(auto& it : aFolder->resources)
		{
			File(it, aFolder);
		}

		for(auto& it : aFolder->unloadedResources)
		{
			const bool open = ImGui::TreeNodeEx((it->name + " (unloaded)").c_str(), ImGuiTreeNodeFlags_Leaf);
			if (ImGui::IsItemHovered())
				CopyDrop(aFolder->path);
			NormalDrop(aFolder->path);
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

void ResourceBrowser::File(EngineResource* aResource, ResourceManager::Folder* aFolder)
{
	if (!aResource)
	{
		if (ImGui::TreeNodeEx("Unable to get resource ptr", ImGuiTreeNodeFlags_Leaf))
			ImGui::TreePop();
		return;
	}

	ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_Leaf;
	const bool selected = GetIsResourceSelected(aResource->myID);
	if (selected)
		flags = ImGuiTreeNodeFlags_Bullet;
	const bool open = ImGui::TreeNodeEx(aResource->myName.c_str(), flags);
	ImGuiDragDropFlags src_flags = 0;
	if (ImGui::BeginDragDropSource(src_flags))
	{
		ImGui::SetDragDropPayload("RESOURCE", &aResource->myID, sizeof(ResourceID));
		ImGui::EndDragDropSource();
	}
	else if (ImGui::IsItemClicked())
	{
		selected ? DeselectResource(aResource) : SelectResource(aResource); // If already selected, deselect. Else select.
		myJustOpened = true;
	}
	else if (ImGui::IsItemHovered())
		CopyDrop(aFolder->path);
	NormalDrop(aFolder->path);
	if (ImGui::BeginPopupContextItem())
	{
		if (LoadedContextMenu(aResource))
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

bool ResourceBrowser::CopyLocal(const std::string& aSrc, const std::string& aDest)
{
	const std::ifstream src(aSrc, std::ios::binary);
	std::ofstream dest(aDest, std::ios::binary);
	dest << src.rdbuf();
	if (!dest || !src)
	{
		Debug::Error << "Unable to copy file " << aSrc << " to " << aDest << std::endl;
		return false;
	}
	return true;
}

bool ResourceBrowser::CopyDrop(const std::string& aDest)
{
	// Actual files
	std::vector<std::wstring> files;
	if (DropManager::GetDragDropFiles(files))
	{
		for (auto& it : files)
		{
			const std::string str(it.begin(), it.end());
			const std::string path = aDest + "/" + ResourceManager::GetNameFromPath(str);
			CopyLocal(str, path);
			myManager->ImportResource(path);
		}
		return true;
	}
	return false;
}

bool ResourceBrowser::NormalDrop(const std::string& aDest)
{
	if (ImGui::BeginDragDropTarget())
	{
		ImGuiDragDropFlags target_flags = 0;
		const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("RESOURCE", target_flags);
		if (payload)
		{
			ResourceID id = *(ResourceID*)payload->Data;
			// Get resource
			// Copy files
			// Edit path in resource class
			auto resource = myManager->GetResource(id);
			if (resource)
			{
				const std::string path = aDest + "/" + resource->myName + ".";
				CopyLocal(resource->myPath, path + resource->myExt);
				std::remove(resource->myPath.c_str());
				if(resource->myExt != "json")
				{
					const std::string jsonPath = ResourceManager::ClipExt(resource->myPath) + ".";
					CopyLocal(jsonPath + "json", path + "json");
					std::remove((jsonPath + "json").c_str());
				}
				resource->myPath = path + resource->myExt;
				myManager->LoadResources();
			}
		}
		ImGui::EndDragDropTarget();
		return payload != nullptr;
	}
	return false;
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
			delete(myResourceViewers[i]);
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
	case ResourceType::SCENE:
		viewer = new SceneViewer(aResource->myType, aResource->myID);
		break;
	case ResourceType::MODEL:
	case ResourceType::TEXTURE:
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

