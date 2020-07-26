#pragma once
#include <typeindex>

class StoredBase
{
public:
	virtual std::type_index GetHash() = 0;
	virtual void* GetData() = 0;
};
