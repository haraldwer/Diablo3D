#include "SceneViewer.h"
#include "../../Engine/Scenes/SceneManager.h"
#include "../../Engine/Engine.h"
#include "imgui.h"

SceneViewer::SceneViewer(const ResourceType& aType, const ResourceID aResourceID) : ResourceViewer(aType, aResourceID)
{
}

void SceneViewer::Update(EngineResource* aResource, Engine* anEngine)
{
	if (ImGui::Button("Load"))
	{
		auto& sceneManager = anEngine->GetServiceLocator().GetService<SceneManager>();
		sceneManager.LoadScene(aResource->myID);
	}
}
