#include "ModelRenderer.h"
#include "../../../D3DX11/D3DModel.h"
#include "../../EngineResources/EngineResource.h"
#include "Engine/EngineResources/ResourceManager.h"

void ModelRenderer::Init()
{
	GetContainer().Iterate([&](const EntityID anID, ModelData& someData)
	{
		return true;
	});
}

void ModelRenderer::Update()
{
	float matrix[16];
	auto& resourceManager = ServiceLocator::Instance().GetService<ResourceManager>();
	GetContainer().Iterate([&](const EntityID anID, ModelData& someData)
	{
		if (someData.myModel)
		{
			someData.GetEntity()->GetTransform().GetMatrix(matrix);
			someData.myModel->SetMatrix(matrix);
			someData.myModel->Render();
		}
		else if(!someData.myHasBeenUpdated)
		{
			someData.myHasBeenUpdated = true;
			// Try to load resource, maybe only do this once?		
			EngineResource* modelResource = resourceManager.GetResource(someData.myModelID.Get());
			EngineResource* textureResource = resourceManager.GetResource(someData.myTextureID.Get());
			if (!modelResource)
			{
				Debug::Error << "Unable to find model resource with id " << someData.myModelID.Get() << std::endl;
				return true;
			}
			if (!textureResource)
			{
				Debug::Error << "Unable to find texture resource with id " << someData.myTextureID.Get() << std::endl;
				return true;
			}
			if (modelResource->myType._value != ResourceType::MODEL)
			{
				Debug::Error << "Model resource " << modelResource->myName << " wasn't of model type" << std::endl;
				return true;
			}
			if (textureResource->myType._value != ResourceType::TEXTURE)
			{
				Debug::Error << "Texture resource " << modelResource->myName << " wasn't of texture type" << std::endl;
				return true;
			}
			someData.myModel = new D3DModel(modelResource->myPath.c_str(), textureResource->myPath.c_str());
			return true;
		}
		return true;
	});
}