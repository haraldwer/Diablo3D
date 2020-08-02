#pragma once
#include "../System.h"
#include "../Data/EntityData.h"
#include "Engine/ECS/Data/Serializable.h"

class D3DModel;

class ModelData : public EntityData
{
	friend class ModelRenderer;
public:
	ModelData() :
	myModelID("myModelID", EditorControls::MODEL),
	myTextureID("myTextureID", EditorControls::TEXTURE),
	myHasBeenUpdated(false),
	myModel(nullptr)
	{
	}
	
private:
	Serializable<ResourceID> myModelID;
	Serializable<ResourceID> myTextureID;
	
	bool myHasBeenUpdated;
	D3DModel* myModel;
};

class ModelRenderer : public System<ModelRenderer, ModelData>
{
public:
	std::string GetName() override { return "Model Renderer"; }
	void Init() override;
	void Update() override;
	void EditorUpdate() override;
};
