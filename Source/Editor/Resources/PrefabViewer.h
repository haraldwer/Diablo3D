#pragma once
#include "ResourceViewer.h"

class PrefabViewer : public ResourceViewer
{
public:
	PrefabViewer(const ResourceType& aType, const ResourceID aResourceID);
	void Update(EngineResource* aResource, Engine* anEngine) override;
private:
};

