#pragma once
#include <typeindex>

class StoredBase
{
public:
	virtual ~StoredBase() = default;
	virtual std::type_index GetHash() = 0;
	virtual void* GetData() = 0;
	virtual void Serialize(rapidjson::Writer<rapidjson::StringBuffer>& aBase) = 0;
};