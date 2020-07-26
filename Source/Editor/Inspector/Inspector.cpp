#include "Inspector.h"
#include "../Viewport.h"
#include "../Hierarchy.h"
#include "../../Engine/ECS/SystemBase.h"
#include "../../Engine/ECS/Data/SerializableBase.h"
#include "../../Engine/ECS/Data/Serializable.h"
#include "EditSerializable.h"

Inspector::Inspector(): mySelectedID("InspectorSelectedID", -1), mySelectedScene("InspectorSelectedSceneID", -1), myEngine(nullptr)
{
	mySerializableFunctions[std::type_index(typeid(int))] =									EditInt;
	mySerializableFunctions[std::type_index(typeid(float))] =								EditFloat;
	mySerializableFunctions[std::type_index(typeid(std::string))] =							EditString;
	mySerializableFunctions[std::type_index(typeid(bool))] =								EditBool;
	mySerializableFunctions[std::type_index(typeid(Vec2F))] =								EditVec2;
	mySerializableFunctions[std::type_index(typeid(Vec3F))] =								EditVec3;
	mySerializableFunctions[std::type_index(typeid(CommonUtilities::Vector4<float>))] =		EditVec4;
	//mySerializableFunctions[std::type_index(typeid(CommonUtilities::Matrix4x4<float>))] =	EditMatrix;
}

bool Inspector::GetIsSelected(SceneID aSceneID, EntityID aEntityID)
{
	return (aSceneID == mySelectedScene.Get() && aEntityID == mySelectedID.Get());
}

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
	c->sceneID = mySelectedScene.Get();
	c->entityID = mySelectedID.Get();
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

void Inspector::EditComponents(Entity* anEntity)
{
	if (!anEntity)
		return;
	
	ServiceLocator& serviceLocator = myEngine->GetServiceLocator();
	auto& sysMan = serviceLocator.GetService<CSystemManager>();
	auto comps = anEntity->GetSystemRefs();
	for(auto& comp : comps)
	{
		auto sys = sysMan.GetSystem(comp);
		if (!sys)
		{
			ImGui::CollapsingHeader("Unable to find system reference");
			continue;
		}
		
		if (ImGui::CollapsingHeader(sys->GetName().c_str(), ImGuiTreeNodeFlags_DefaultOpen))
		{
			ImGui::PushID(sys);
			auto& properties = sys->GetEntityProperties(anEntity->GetID());
			for(auto& it : properties)
			{
				auto type = it.second->GetType();
				if (mySerializableFunctions[type])
					mySerializableFunctions[type](it.second);
			}
			ImGui::PopID();
		}
	}
}

void Inspector::Update(Gizmo& aGizmo, Engine* anEngine)
{
	myEngine = anEngine;
	if (!myEngine)
		return;
	
	Entity* entity = GetEntity(mySelectedScene.Get(), mySelectedID.Get());
	if(!entity)
	{
		mySelectedScene.Set(-1);
		mySelectedID.Set(-1);
		ImGui::Text("No entity selected");
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

	EditComponents(entity);

	if (ImGui::IsWindowFocused() || Viewport::IsFocused() || Hierarchy::IsFocused())
	{
		Input& input = myEngine->GetServiceLocator().GetService<Input>();
		if (!Hierarchy::IsFocused() && input.Get(VK_CONTROL))
		{
			if (input.GetPressed('Z'))
				myCommandQueue.Revert();
			if (input.GetPressed('Y'))
				myCommandQueue.Redo();
			if (input.GetPressed('S'))
				Save();
			if (input.GetPressed('D'))
				Duplicate();
		}
		if (input.GetPressed(VK_DELETE))
		{
			entity->Destroy();
			SetEntity(-1, -1);
		}
	}
}

void Inspector::SetEntity(const SceneID aScene, const EntityID aEntity)
{
	mySelectedID.Set(aEntity);
	mySelectedScene.Set(aScene);
}

void Inspector::Save()
{
	ServiceLocator& serviceLocator = myEngine->GetServiceLocator();
	SceneManager& sceneManager = serviceLocator.GetService<SceneManager>();
	Scene* scene = sceneManager.GetScene(mySelectedScene.Get());
	if (!scene)
	{
		Debug::Error << "Unable to save scene, scene ptr was null" << std::endl;
		return;
	}
	scene->Save();
}

void Inspector::Duplicate()
{
	if(mySelectedID.Get() == -1 || mySelectedScene.Get() == -1)
	{
		Debug::Error << "Unable to duplicate entity, no entity selected" << std::endl;
		return;
	}
	ServiceLocator& serviceLocator = myEngine->GetServiceLocator();
	SceneManager& sceneManager = serviceLocator.GetService<SceneManager>();
	Scene* scene = sceneManager.GetScene(mySelectedScene.Get());
	if(!scene)
	{
		Debug::Error << "Unable to find scene with ID " << mySelectedScene.Get() << std::endl;
		return;
	}
	Entity* entity = GetEntity(mySelectedScene.Get(), mySelectedID.Get());
	if (!scene)
	{
		Debug::Error << "Unable to find entity with ID " << mySelectedID.Get() << std::endl;
		return;
	}
	auto duplicate = scene->CreateEntity(entity->GetPrefabID());
	duplicate->GetTransform().SetMatrix(entity->GetTransform().GetMatrix());
	// Also transfer component overrides!

	Debug::Log << "Entity duplicated" << std::endl;
}