#pragma once
#include <functional>
#include <vector>
#include "../../CommonUtilities/OpaqueDictionary.hpp"
#include "../../Engine/Entity/Entity.h"
#include "../../Engine/Scenes/Scene.h"

struct Command
{
	enum class Type
	{
		TRANSFORM,
		PROPERTY
	};

	EntityID entityID;
	SceneID sceneID;
	CommonUtilities::OpaqueDictionary<int> data;
	std::function<void(Command& command)> revert;
	std::function<void(Command& command)> redo;
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

