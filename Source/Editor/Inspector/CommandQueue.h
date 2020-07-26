#pragma once
#include <functional>
#include <vector>
#include "../../CommonUtilities/OpaqueDictionary.hpp"
#include "../../Engine/ResouceIDs.h"

struct Command
{
	enum class Type
	{
		TRANSFORM,
		PROPERTY,
		ENTITY
	};

	EntityID entityID;
	SceneID sceneID;
	CommonUtilities::OpaqueDictionary<int> data;
	std::function<void(Command& command)> revert;
	std::function<void(Command& command)> redo;
	std::function<void(Command& command)> erase;
};


class CommandQueue
{
public:
	void Add(Command* aCommand);
	void Revert();
	void Redo();
	
private:
	int off = 0;
	std::vector<Command*> myCommands;
};

