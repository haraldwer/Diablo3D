#include "Prefab.h"

std::vector<std::string> Prefab::GetComponents() const
{
	return myComponents;
}

std::string Prefab::GetName() const
{
	return myName;
}
