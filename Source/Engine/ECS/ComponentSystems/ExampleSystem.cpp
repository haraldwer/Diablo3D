#include "ExampleSystem.h"

void ExampleSystem::Init()
{
	// This is the init call FOR THE ENTIRE SYSTEM
	// This is called when the engine starts and NOT upon a scene change
	// No entities have been added at this point
}

void ExampleSystem::Update()
{
	/*
	
	Introduction
	- This is the update call FOR THE ENTIRE SYSTEM !!!
	- You have to update each entity manually.
	- EntityData is stored in a container-class called "CContainer".
	- Every system has its own container and the container is managed automatically.

	
	CContainer basics
	- To access the container, use the function "GetContainer()".
	- The container has some useful functions that you'll soon become familiar with.
		+ Iterate()
		+ Perform()
		+ HasData() and GetData()
	
	 */


	
	// Iterate()
	// This function is going to iterate through every ACTIVE entity
	// and perform the function that you give it.
	GetContainer().Iterate([&](const EntityID anEntityID, ExampleData& someData)
	{
		// You'll usually want to call this function once every frame, since it's the same as a normal update.
		// This function will be performed once for each entity.
		// "someData" is the data of this particular entity

		someData.myUnsavedVariable++;
		
		return true; // Return false if you want to break out of the loop
	});


	
	// Perform()
	// This function is similar to Iterate() and will try to perform a function on the specified entity
	// Perform() will return false if it failed to perform the function or if the function returned false
	const EntityID anEntityID = 10; // Of course, we don't know the ID's of any entities during compile time, so this is going to fail.
	GetContainer().Perform(anEntityID, [&](const EntityID anEntityID, ExampleData& someData)
	{
		return true; // Return false if failure
	});


	
	// HasData() & GetData()
	// Use this function to access the data of a particular entity.
	// It's important to use "&" since it's a reference we want.
	if(GetContainer().HasData(anEntityID))
	{
		ExampleData& data = GetContainer().GetData(anEntityID);
		data.myUnsavedVariable = 0; // Now we're able to access the data of this particular entity
	}

}

void ExampleSystem::EditorUpdate()
{
	Debug::Log << "Editor update called" << std::endl;
}


// This is just an example on how to write your own ImGui controls
#include "ImGui/imgui.h"

void ExampleSystem::EditSystem()
{
	ImGui::Text("Editing examplesystem");
}

void ExampleSystem::EditPrefab(const PrefabID anID)
{
	ImGui::Text("Editing example prefab");
}

void ExampleSystem::EditEntity(const EntityID anID)
{
	ImGui::Text("Editing example entity");
}
