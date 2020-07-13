#pragma once
#include "Inspector/Inspector.h"

class Hierarchy
{
public:
	Hierarchy(Inspector& anInspector);
	void Init();
	void Update(Engine* anEngine);

private:
	Inspector& myInspector;
};

