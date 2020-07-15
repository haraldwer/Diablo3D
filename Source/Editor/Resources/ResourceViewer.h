#pragma once
#include "../../Engine/EngineResources/EngineResource.h"
class Engine;

class ResourceViewer
{
public:
	ResourceViewer(const ResourceType& aType, const ResourceID aResourceID);
	virtual ~ResourceViewer() = default;
	bool WindowUpdate(Engine* anEngine);
	virtual void Update(EngineResource* aResource, Engine* anEngine) {}
	ResourceID GetResourceID() const;;
	bool IsOpen() const;
	
private:
	bool myIsOpen;
	const ResourceID myResourceID;
	const ResourceType myResourceType;
};

