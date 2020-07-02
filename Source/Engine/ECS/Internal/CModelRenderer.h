#pragma once
#include "../CSystem.h"
#include "../CContainer.h"

class D3DModel;

class CModelRenderer : public CSystem
{
	struct Data
	{
		D3DModel* myModel;
	};
	struct PrefabData
	{
		std::string myModelName;
		std::string myTextureName;
	};
	
public:
	void Init() override;
	void Update() override;
	std::string GetSystemName() override;
private:
	CContainer<Data, PrefabData> myContainer;
};

