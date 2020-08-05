#pragma once
#include <string>
#include "../CommonUtilities/BetterEnums.h"

#define BETTER_ENUMS_DEFAULT_CONSTRUCTOR(Enum) \
  public:                                      \
	Enum() = default;

BETTER_ENUM(ResourceType, int,
	PREFAB,
	SCENE,
	MODEL,
	TEXTURE,
	UNKNOWN
)

typedef int ResourceID;

class EngineResource
{
public:
	std::string myName;
	std::string myPath;
	std::string myExt;
	std::string myDoc;
	ResourceType myType;
	ResourceID myID;
};

