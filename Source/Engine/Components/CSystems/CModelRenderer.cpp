#include "CModelRenderer.h"
#include "../../../D3DX11/D3DModel.h"

void CModelRenderer::Init()
{
	myContainer.Iterate([&](EntityID anID, Data& someData, const PrefabData& somePrefabData)
	{
		someData.myModel = new D3DModel(somePrefabData.myModelName.c_str(), somePrefabData.myTextureName.c_str());
		return true;
	});
}

void CModelRenderer::Update()
{
	myContainer.Iterate([&](EntityID anID, Data& someData, const PrefabData& somePrefabData)
	{
		if (someData.myModel)
			someData.myModel->Render();
		return true;
	});
}

std::string CModelRenderer::GetSystemName()
{
	return "Model renderer";
}
