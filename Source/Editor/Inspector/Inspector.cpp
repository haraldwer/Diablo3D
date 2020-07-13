#include "Inspector.h"

Entity* Inspector::GetEntity(SceneID sceneID, EntityID entityID)
{
	if (!myEngine)
		return nullptr;
	if (sceneID == -1)
		return nullptr;
	if (entityID == -1)
		return nullptr;
	ServiceLocator& serviceLocator = myEngine->GetServiceLocator();
	SceneManager& sceneManager = serviceLocator.GetService<SceneManager>();
	Scene* scene = sceneManager.GetScene(sceneID);
	if (!scene)
		return nullptr;
	Entity* entity = scene->GetEntity(entityID);
	if (!entity)
		return nullptr;
	return entity;
}

void Inspector::EditTranform(Transform& aTransform)
{
	Command* c = new Command();
	c->sceneID = mySelectedScene;
	c->entityID = mySelectedID;
	c->data.SetValue<Vec3F>(0, aTransform.GetPosition());
	c->data.SetValue<Vec3F>(1, aTransform.GetRotation());
	c->data.SetValue<Vec3F>(2, aTransform.GetScale());
	c->revert = [&](Command& command)
	{
		Entity* e = GetEntity(command.sceneID, command.entityID);
		if (!e)
			return;
		
		Vec3F tempPos = e->GetTransform().GetPosition();
		Vec3F tempRot = e->GetTransform().GetRotation();
		Vec3F tempScale = e->GetTransform().GetScale();

		e->GetTransform().SetPosition(command.data.GetValue<Vec3F>(0));
		e->GetTransform().SetRotation(command.data.GetValue<Vec3F>(1));
		e->GetTransform().SetScale(command.data.GetValue<Vec3F>(2));

		command.data.SetValue<Vec3F>(0, tempPos);
		command.data.SetValue<Vec3F>(1, tempRot);
		command.data.SetValue<Vec3F>(2, tempScale);
	};
	c->redo = c->revert;
	myCommandQueue.Add(c);
}

void Inspector::Update(Gizmo& aGizmo, Engine* anEngine)
{
	myEngine = anEngine;
	if (!myEngine)
		return;
	
	Input& input = myEngine->GetServiceLocator().GetService<Input>();
	if (input.Get(VK_CONTROL))
	{
		if(input.GetPressed('Z'))
			myCommandQueue.Revert();
		if (input.GetPressed('Y'))
			myCommandQueue.Redo();
	}
	
	Entity* entity = GetEntity(mySelectedScene, mySelectedID);
	if(!entity)
	{
		mySelectedScene = -1;
		mySelectedID = -1;
		return;
	}
	
	if (aGizmo.Manipulate(entity->GetTransform()))
	{
		if(!myIsManipulating)
		{
			EditTranform(entity->GetTransform());
			myIsManipulating = true;
		}
	}
	else
		if(myIsManipulating)
			myIsManipulating = false;
}

void Inspector::SetEntity(const SceneID aScene, const EntityID aEntity)
{
	mySelectedID = aEntity;
	mySelectedScene = aScene;
}
