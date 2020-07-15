#pragma once
#include "Inspector/Inspector.h"

class Hierarchy
{
public:
	Hierarchy(Inspector& anInspector);
	void Init();
	void Update(Engine* anEngine);
	static bool IsFocused();
private:
	static bool myIsFocused;
	Inspector& myInspector;
};

