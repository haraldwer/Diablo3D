#include "CModelRenderer.h"
#include "../../../D3DX11/D3DModel.h"
#include <iostream>
#include "Log.h"

void CModelRenderer::Init()
{
	myContainer.Iterate([&](const EntityID anID, Data& someData, const PrefabData& somePrefabData)
	{
		someData.myModel = new D3DModel(somePrefabData.myModelName.c_str(), somePrefabData.myTextureName.c_str());
		return true;
	});
	Debug::Log << "Test" << std::endl;
}

void CModelRenderer::Update()
{
	int iterations = 0;
	myContainer.Iterate([&](const EntityID anID, Data& someData, const PrefabData& somePrefabData)
	{
		iterations++;
		if (someData.myModel)
			someData.myModel->Render();
		return true;
	});
	Debug::Log << "Iterations: " << iterations << std::endl;
}

std::string CModelRenderer::GetSystemName()
{
	return "Model renderer";
}
