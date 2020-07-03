#pragma once
#include "../CSystem.h"
#include "../CContainer.h"
#include "RapidJSON/document.h"

class D3DModel;

class CModelRenderer : public CSystem<CModelRenderer>
{
	struct Data
	{
		D3DModel* myModel;
	};
	struct PrefabData
	{
		ResourceID myModelID;
		ResourceID myTextureID;
	};
	
public:
	std::string GetSystemName() override { return "Model Renderer"; }
private:
	void Init() override;
	void Update() override;
	void AddEntity(Entity* anEntity) override;
	void LoadPrefab(PrefabID aPrefabID, const rapidjson::GenericObject<false, rapidjson::Value>& aBase) override;
	size_t GetEntityCount() override;
	CContainer<Data, PrefabData> myContainer;
};

