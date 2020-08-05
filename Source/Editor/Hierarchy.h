#pragma once
#include "Inspector/Inspector.h"

class Hierarchy
{
public:
	Hierarchy(Inspector& anInspector);
	void Init();
	void Update(Engine* anEngine);
	static bool IsFocused();
	void DragDrop(SceneID aSceneID, EntityID anEntityID, Engine* anEngine);
	void CreateEmpty(SceneID aSceneID, Engine* anEngine);
private:
	static bool myIsFocused;
	Inspector& myInspector;
};

