#include "PrefabViewer.h"
#include "../../Engine/Engine.h"
#include "../../Engine/ECS/SystemBase.h"
#include "../../Engine/ECS/Data/StoredBase.h"
#include "../../ImGui/imgui.h"

PrefabViewer::PrefabViewer(const ResourceType& aType, const ResourceID aResourceID) : ResourceViewer(aType, aResourceID)
{
}

void PrefabViewer::Update(EngineResource* aResource, Engine* anEngine)
{
	if (!aResource)
		return;
	if (!anEngine)
		return;
	auto& serviceLocator = anEngine->GetServiceLocator();
	PrefabManager& prefabMan = serviceLocator.GetService<PrefabManager>();
	CSystemManager& sysMan = serviceLocator.GetService<CSystemManager>();

	Prefab* prefab = prefabMan.GetPrefab(aResource->myID);
	if (!prefab)
		return;
	auto components = prefab->GetComponents();
	for(auto& it : components)
	{
		SystemBase* sys = sysMan.GetSystem(it);
		if (!sys)
			continue;
		ImGui::PushID(it.c_str());
		if(ImGui::CollapsingHeader(it.c_str(), ImGuiTreeNodeFlags_DefaultOpen))
		{
			auto& properties = sys->GetPrefabProperties(aResource->myID);
			for(auto& prop : properties)
			{
				ImGui::Text(prop.first.c_str());
			}
		}
		ImGui::PopID();
	}
}
