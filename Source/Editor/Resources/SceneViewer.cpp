#include "SceneViewer.h"
#include "../../Engine/Scenes/SceneManager.h"
#include "../../Engine/Engine.h"
#include "imgui.h"

SceneViewer::SceneViewer(const ResourceType& aType, const ResourceID aResourceID) : ResourceViewer(aType, aResourceID)
{
}

void SceneViewer::Update(EngineResource* aResource, Engine* anEngine)
{
	auto& sceneManager = anEngine->GetServiceLocator().GetService<SceneManager>();

	if (ImGui::Button("Load"))
		sceneManager.LoadScene(aResource->myID);
}
