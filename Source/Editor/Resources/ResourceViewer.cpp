#include "ResourceViewer.h"
#include "../ImGui/imgui.h"
#include "../Editor.h"

ResourceViewer::ResourceViewer(const ResourceType& aType, const ResourceID aResourceID) : myIsOpen(true), myResourceID(aResourceID), myResourceType(aType)
{
}

bool ResourceViewer::WindowUpdate(Engine* anEngine)
{
	if (!anEngine)
		return true;
	
	if (!myIsOpen)
		return false;
	
	if(ImGui::Begin((myResourceType._to_string() + std::string(" Resource Editor")).c_str(), &myIsOpen))
	{
		EngineResource* resource = anEngine->GetServiceLocator().GetService<ResourceManager>().GetResource(myResourceID);
		if(resource)
		{
			ImGui::Text(("ID: " + std::to_string(resource->myID)).c_str());
			ImGui::Text(("Name: " + resource->myName).c_str());
			ImGui::Text(("Ext: " + resource->myExt).c_str());
			ImGui::Text(("Path: " + resource->myPath).c_str());
			Update(resource, anEngine);
		}
		else
		{
			ImGui::Text(("Unable to load resource with id " + std::to_string(myResourceID)).c_str());
		}
	}
	ImGui::End();
	return myIsOpen;
}

ResourceID ResourceViewer::GetResourceID() const
{
	return myResourceID;
}

bool ResourceViewer::IsOpen() const
{
	return myIsOpen;
}
