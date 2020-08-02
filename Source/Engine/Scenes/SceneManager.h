#pragma once
#include "Scene.h"
#include "../Utility/ServiceLocator.h"

class EngineResource;

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
	Scene* GetFirstScene();
	std::vector<SceneID> GetLoadedScenes();
	void SaveScenes();
private:
	SceneID LoadScene(EngineResource* aSceneResource);
	std::unordered_map<SceneID, Scene*> myScenes;
};
