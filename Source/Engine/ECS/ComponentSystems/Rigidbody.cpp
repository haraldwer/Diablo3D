#include "Rigidbody.h"

void Rigidbody::Init()
{
}

void Rigidbody::Update()
{
	GetContainer().Iterate([&](const EntityID anEntityID, RBData& someData)
	{
		return true;
	});
}
