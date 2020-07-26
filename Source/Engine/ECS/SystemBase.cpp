#include "SystemBase.h"
#include "Engine/Scenes/SceneManager.h"

EntityID SystemBase::Instantiate(PrefabID aPrefabID, SceneID aSceneID)
{
	auto& sceneMan = ServiceLocator::Instance().GetService<SceneManager>();
	Scene* scene = nullptr;
	if(aSceneID == -1)
	{
		scene = sceneMan.GetFirstScene();
	}
	else
	{
		scene = sceneMan.GetScene(aSceneID);
	}
	if (!scene)
		return -1;
	Entity* entity = scene->CreateEntity(aPrefabID);
	if (!entity)
		return -1;
	return entity->GetID();
}

EntityID SystemBase::Instantiate(const std::string& aPrefabName, SceneID aSceneID)
{
	auto& prefabMan = ServiceLocator::Instance().GetService<PrefabManager>();
	const PrefabID id = prefabMan.GetPrefabID(aPrefabName);
	if(id == -1)
		return -1;
	return Instantiate(id, aSceneID);
}
