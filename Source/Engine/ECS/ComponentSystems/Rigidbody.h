#pragma once
#include "Engine/ECS/Data/EntityData.h"
#include "Engine/ECS/System.h"

class RBData : public EntityData
{
	
};

class Rigidbody : public System<Rigidbody, RBData>
{
public:
	std::string GetName() override { return "Rigidbody"; };
	void Init() override;
	void Update() override;
};
