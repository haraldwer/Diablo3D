#include "CModelRenderer.h"
#include "../../../D3DX11/D3DModel.h"
#include "../../EngineResources/EngineResource.h"
#include "Engine/EngineResources/ResourceManager.h"

void CModelRenderer::Init()
{
	myContainer.Iterate([&](const EntityID anID, Data& someData, const PrefabData& somePrefabData)
	{
		return true;
	});
}

void CModelRenderer::Update()
{
	myContainer.Iterate([&](const EntityID anID, Data& someData, const PrefabData& somePrefabData)
	{
		if (someData.myModel && someData.myEntity)
		{
			const Vec3F pos = someData.myEntity->GetTransform().GetPosition();
			const Vec3F rot = someData.myEntity->GetTransform().GetRotation();
			const Vec3F scale = someData.myEntity->GetTransform().GetScale();
			someData.myModel->SetPosition(pos.x, pos.y, pos.z);
			someData.myModel->SetRotation(rot.x, rot.y, rot.z);
			someData.myModel->SetScale(scale.x, scale.y, scale.z);
			someData.myModel->Render();
		}
		return true;
	});
}

void CModelRenderer::AddEntity(Entity* anEntity)
{
	if (anEntity)
	{
		Data& data = myContainer.Insert(*anEntity);
		data.myEntity = anEntity;
		myContainer.Perform(anEntity->GetID(), [&](const EntityID anID, Data& someData, const PrefabData& somePrefabData)
		{
			ResourceManager& resourceManager = ServiceLocator::Instance().GetService<ResourceManager>();
			EngineResource* modelResource = resourceManager.GetResource(somePrefabData.myModelID);
			EngineResource* textureResource = resourceManager.GetResource(somePrefabData.myTextureID);

			if (!modelResource)
			{
				Debug::Error << "Unable to find model resource. " << std::endl;
				return true;
			}
			if (!textureResource)
			{
				Debug::Error << "Unable to find texture resource. " << std::endl;
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

		});
	}
}

void CModelRenderer::RemoveEntity(EntityID anEntityID)
{
	myContainer.RemoveData(anEntityID);
}

void CModelRenderer::LoadPrefab(PrefabID aPrefabID, const rapidjson::GenericObject<false, rapidjson::Value>& aBase)
{
	PrefabData data;
	data.myModelID = aBase["Model"].GetInt();
	data.myTextureID = aBase["Texture"].GetInt();
	myContainer.InsertPrefabData(aPrefabID, data);
}

size_t CModelRenderer::GetEntityCount()
{
	return myContainer.GetEntityCount();
}
