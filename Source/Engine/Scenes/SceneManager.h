#pragma once
#include "Scene.h"
#include "../Utility/ServiceLocator.h"

class SceneManager : public Service<SceneManager>
{
public:
	void Init();
	SceneID LoadScene(std::string aScene);
	SceneID LoadScene(ResourceID aSceneID);
	bool UnloadScene(const std::string& aScene);
	bool UnloadScene(SceneID aSceneID);
	std::string GetName() override { return "Scene Manager"; }
	void Editor() override;
	Scene* GetScene(SceneID aSceneID);
	std::vector<SceneID> GetLoadedScenes();
private:
	SceneID LoadScene(EngineResource* aSceneResource);
	std::unordered_map<SceneID, Scene*> myScenes;
};
