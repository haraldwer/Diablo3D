#include "SceneManager.h"

void SceneManager::Init()
{
}

Scene* SceneManager::CreateScene()
{
	Scene* scene = new Scene(myScenes.size(), "");
	myScenes.push_back(scene);
	return scene;
}

Scene* SceneManager::LoadScene(const std::string& aPath)
{
	return nullptr;
}
