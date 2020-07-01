#pragma once
#include <string>

class CSystem
{
public:
	CSystem();
	virtual ~CSystem() = default;
	virtual void Init() = 0;
	virtual void Update() = 0;
	virtual std::string GetSystemName() = 0;
	size_t GetHash() const;
private:
	size_t myHash;
	// CContainer<CData, CPrefabData> myContainer;
};

