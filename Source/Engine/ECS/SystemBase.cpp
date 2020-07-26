#include "SystemBase.h"
#include "Engine/Scenes/SceneManager.h"

EntityID SystemBase::Instantiate(PrefabID aPrefabID, SceneID aSceneID)
{
	auto& sceneMan = ServiceLocator::Instance().GetService<SceneManager>();
	Scene* scene;
	if(aSceneID == -1)
	{
		
	}
	else
	{
		scene = sceneMan.GetScene(aSceneID);
	}
	
}

EntityID SystemBase::Instantiate(const std::string& aPrefabName, SceneID aSceneID)
{
	auto& prefabMan = ServiceLocator::Instance().GetService<PrefabManager>();
	PrefabID id = prefabMan.GetPrefabID(aPrefabName);
	if(id == -1)
		return -1;
	return Instantiate(id, aSceneID);
}
