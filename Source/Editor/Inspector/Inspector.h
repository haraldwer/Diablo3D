#pragma once
#include "Gizmo.h"
#include "../Engine/Engine.h"
#include "CommandQueue.h"
#include "../Preferences.h"
#include "../../Engine/ECS/Data/Serializable.h"

class SerializableBase;

class Inspector
{
public:
	Inspector();
	void Update(Gizmo& aGizmo, Engine* anEngine);
	void SetEntity(const SceneID aScene, const EntityID aEntity);
	bool GetIsSelected(SceneID aSceneID, EntityID aEntityID);
private:
	Entity* GetEntity(const SceneID aScene, const EntityID aEntity);
	void EditTranform(Transform& aTransform);
	void EditComponents(Entity* anEntity);
	void Duplicate();
	void Save();
	
	Preference<EntityID> mySelectedID;
	Preference<SceneID> mySelectedScene;
	bool myIsManipulating = false;

	Engine* myEngine;
	CommandQueue myCommandQueue;

	std::unordered_map<std::type_index, std::function<void(SerializableBase* aBase, CommandQueue& aQueue)>> mySerializableFunctions;

	template <class T>
	void Edit(Serializable<T>* ptr, T& val, CommandQueue& queue);
	template <class T>
	void Apply(Serializable<T>* ptr);
	
	void EditInt(SerializableBase* base, CommandQueue& queue);
	void EditFloat(SerializableBase* base, CommandQueue& queue);
	void EditBool(SerializableBase* base, CommandQueue& queue);
	void EditString(SerializableBase* base, CommandQueue& queue);
	void EditVec2(SerializableBase* base, CommandQueue& queue);
	void EditVec3(SerializableBase* base, CommandQueue& queue);
	void EditVec4(SerializableBase* base, CommandQueue& queue);
};

template <class T>
void Inspector::Edit(Serializable<T>* ptr, T& val, CommandQueue& queue)
{
	auto c = new Command();
	c->data.SetValue<std::string>(0, ptr->GetName());
	c->data.SetValue<T>(1, ptr->Get());
	c->entityID = mySelectedID.Get();
	c->sceneID = mySelectedScene.Get();
	c->revert = [&](Command& aCommand)
	{
		
	};
	c->redo = [&](Command& aCommand)
	{

	};
	queue.Add(c);
	ptr->Set(val);
}

template <class T>
void Inspector::Apply(Serializable<T>* ptr)
{
	if (!ptr->IsOverride())
		return;
	if (!ImGui::BeginPopupContextItem())
		return;
	if (ImGui::MenuItem("Apply"))
		ptr->Apply();
	if (ImGui::MenuItem("Revert"))
		ptr->Revert();
	ImGui::EndPopup();
}