#pragma once
#include "ResourceViewer.h"

class SceneViewer : public ResourceViewer
{
public:
	SceneViewer(const ResourceType& aType, const ResourceID aResourceID);
	void Update(EngineResource* aResource, Engine* anEngine) override;
private:
};

