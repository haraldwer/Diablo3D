#pragma once
#include <unordered_map>
#include <string>
#include "../../Utility/JSONHelper.h"
#include "Stored.h"

class PrefabData
{
public:
	PrefabData();
	PrefabData(const PrefabData& anOther);
	void Serialize(rapidjson::Writer<rapidjson::StringBuffer>& aBase);
	void Deserialize(const rapidjson::Value::Object& aContainer);
	template <class T>
	Stored<T>* GetStored(const std::string& aStoredName);
	void Clear();
	
private:
	std::unordered_map<std::string, StoredBase*> myStoredMap;
};

template <class T>
Stored<T>* PrefabData::GetStored(const std::string& aStoredName)
{
	auto f = myStoredMap.find(aStoredName);
	if (f == myStoredMap.end())
	{
		auto* s = new Stored<T>();
		myStoredMap[aStoredName] = s;
		return s;
	}
	return reinterpret_cast<Stored<T>*>(f->second);
}
