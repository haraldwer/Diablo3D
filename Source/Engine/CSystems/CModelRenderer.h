#pragma once
#include "../CSystem.h"

class D3DModel;

class CModelRenderer : public CSystem
{
public:
	CModelRenderer();
	void Init() override;
	void Update() override;
	std::string GetSystemName() override;
private:
};

