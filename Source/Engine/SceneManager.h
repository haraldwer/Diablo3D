#pragma once
#include <vector>
#include "Scene.h"

class SceneManager
{
public:
	void Init();
	Scene* CreateScene();
	Scene* LoadScene(const std::string& aPath);
private:
	std::vector<Scene*> myScenes;
};
