#pragma once
#include "Scene.h"
#include "../Utility/ServiceLocator.h"

class SceneManager : public Service<SceneManager>
{
public:
	void Init();
	SceneID LoadScene(std::string aScene);
	SceneID LoadScene(ResourceID aSceneID);
	std::string GetName() override { return "Scene Manager"; }
private:
	SceneID LoadScene(EngineResource* aSceneResource);
	std::unordered_map<SceneID, Scene*> myScenes;
};
