
// Welcome to the ExampleSystem!
// Let this be your guide when creating your own systems!
//
// Don't forget to add the system in "CSystems.h"!
//
// Good luck!

#pragma once
#include "Engine/ECS/System.h" 

class ExampleData : public EntityData // Don't forget this inheritance!
{

	// I recommend using "friend" in this situation,
	// since we want to easily access the private members of this class from our system.
	friend class ExampleSystem;
	
public:
	ExampleData() :
	myFirstExampleData("FirstExampleVariable"),									// "aName" is the name that is shown in the editor and stored in the json
	mySecondExampleData("SecondExampleVariable", EditorControls::DRAG_INT),		// The EditorControls enum defines how the control should look in the editor, but doesn't have to be set. To hide a variable in the editor, use "NONE"
	SERIALIZABLE(myThirdExampleData),												// The SERIALIZABLE enum does the same as the first example, but the name in editor will be the same as the variable name, in this case "myThirdExampleData"
	myUnsavedVariable(0)
	{
	}

	// I advice against putting functions in this class
	
private:
	// Serializable variable will be saved, loaded and shown in the editor automatically,
	// but they have to be added to the constructor-list above.
	Serializable<int> myFirstExampleData;
	Serializable<int> mySecondExampleData;
	Serializable<int> myThirdExampleData;

	int myUnsavedVariable; // Normal variables works as they usually do.
};

class ExampleSystem : public System<ExampleSystem, ExampleData>
									// ^ This should be replaced with your own System and Data class
{
public:
	
private:
	void Init() override;			// These two are necessary 
	void Update() override;			//

	void EditorUpdate() override;

	// The following functions allow you to write your own ImGui controls
	void EditSystem() override;
	void EditPrefab(const PrefabID anID) override;
	void EditEntity(const EntityID anID) override;
};

