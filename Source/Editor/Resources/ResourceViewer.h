#pragma once
#include "../../Engine/EngineResources/EngineResource.h"
class Engine;

class ResourceViewer
{
public:
	ResourceViewer(const ResourceType& aType);
	virtual ~ResourceViewer() = default;
	void WindowUpdate(Engine* anEngine);
	virtual void Update(EngineResource* aResource, Engine* anEngine) {};
	virtual void Select(ResourceID anID);
private:
	bool myIsOpen;
	ResourceID myResourceID;
	const ResourceType myResourceType;
};

