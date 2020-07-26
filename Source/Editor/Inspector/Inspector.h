#pragma once
#include "Gizmo.h"
#include "../Engine/Engine.h"
#include "CommandQueue.h"
#include "../Preferences.h"
#include "../../Engine/ECS/SystemBase.h"
#include "../../Engine/ECS/Data/Serializable.h"

class SerializableBase;

class Inspector
{
public:
	Inspector();
	void Update(Gizmo& aGizmo, Engine* anEngine);
	void SetEntity(const SceneID aScene, const EntityID aEntity);
	bool GetIsSelected(SceneID aSceneID, EntityID aEntityID);
	void Duplicate();
	void Delete();
	void HideShowCommand(bool aPerform);
private:
	Entity* GetEntity(const SceneID aScene, const EntityID aEntity) const;
	void EditTranform(Transform& aTransform);
	void EditComponents(Entity* anEntity);
	void Save();
	
	Preference<EntityID> mySelectedID;
	Preference<SceneID> mySelectedScene;
	bool myIsManipulating = false;

	Engine* myEngine;
	CommandQueue myCommandQueue;

	std::unordered_map<std::type_index, std::function<void(SerializableBase* aBase, CommandQueue& aQueue, const std::string& aSystemName)>> mySerializableFunctions;

	template <class T>
	void Edit(Serializable<T>* ptr, T& val, CommandQueue& queue, const std::string& aSystemName);
	template <class T>
	void Apply(Serializable<T>* ptr);
	
	void EditInt(SerializableBase* base, CommandQueue& queue, const std::string& aSystemName);
	void EditFloat(SerializableBase* base, CommandQueue& queue, const std::string& aSystemName);
	void EditBool(SerializableBase* base, CommandQueue& queue, const std::string& aSystemName);
	void EditString(SerializableBase* base, CommandQueue& queue, const std::string& aSystemName);
	void EditVec2(SerializableBase* base, CommandQueue& queue, const std::string& aSystemName);
	void EditVec3(SerializableBase* base, CommandQueue& queue, const std::string& aSystemName);
	void EditVec4(SerializableBase* base, CommandQueue& queue, const std::string& aSystemName);
};

template <class T>
void Inspector::Edit(Serializable<T>* ptr, T& val, CommandQueue& queue, const std::string& aSystemName)
{
	auto c = new Command();
	c->data.SetValue<std::string>(0, ptr->GetName());
	c->data.SetValue<T>(1, ptr->Get());
	c->data.SetValue<std::string>(2, aSystemName);
	c->entityID = mySelectedID.Get();
	c->sceneID = mySelectedScene.Get();
	c->revert = [&](Command& aCommand)
	{
		if (!myEngine)
			return;
		auto& locator = myEngine->GetServiceLocator();
		auto& sysMan = locator.GetService<CSystemManager>();
		const auto sysName = aCommand.data.GetValue<std::string>(2);
		auto sys = sysMan.GetSystem(sysName);
		if (!sys)
			return;
		const auto propertyName = aCommand.data.GetValue<std::string>(0);
		SerializableBase* base = sys->GetEntityProperty(aCommand.entityID, propertyName);
		if (!base)
			return;
		auto ptr = reinterpret_cast<Serializable<T>*>(base);
		T val = aCommand.data.GetValue<T>(1);
		aCommand.data.SetValue(1, ptr->Get());
		ptr->Set(val);
	};
	c->redo = c->revert;
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