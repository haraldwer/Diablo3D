#pragma once
#include <unordered_map>
#include <functional>
#include <thread>
#include <mutex>
#include "../Entity.h"

template <class CData, class CPrefabData>
class CContainer
{
public:
	// Iterates through all components and performs function on their data. Returns false if it failed
	bool Iterate(std::function<bool(EntityID anEntityID, CData& someData, const CPrefabData& somePrefabData)> aFunction);
	bool IterateThread(std::function<bool(EntityID anEntityID, CData& someData, const CPrefabData& somePrefabData)> aFunction);
	void InsertData(const EntityID anID, const CData& someData);
	void InsertPrefabData(const int anID, const CPrefabData& someData);
	void RemoveData(const EntityID anID);
	CData& GetData(const EntityID anID);
	CPrefabData& GetPrefabData(const int anID);
private:
	std::unordered_map<EntityID, CData> myData;
	std::unordered_map<int, CPrefabData> myPrefabData;
};

template <class CData, class CPrefabData>
bool CContainer<CData, CPrefabData>::Iterate(std::function<bool(EntityID anEntityID, CData& someData, const CPrefabData& somePrefabData)> aFunction)
{
	for (auto& it : myData)
		if (!aFunction(it.first, it.second, myPrefabData[0]))
			return false;
	return true;
}

template <class CData, class CPrefabData>
bool CContainer<CData, CPrefabData>::IterateThread(std::function<bool(EntityID anEntityID, CData& someData, const CPrefabData& somePrefabData)> aFunction)
{
	//may return 0 when not able to detect
	const auto processor_count = std::thread::hardware_concurrency();
	if (processor_count == 0)
		Iterate(aFunction);
	else
	{
		auto itr = myData.begin();
		std::mutex mutex;
		
		std::vector<std::thread*> threads;
		for(int i = 0; i < processor_count; i++)
		{
			std::thread* thread = new std::thread([&]
			{
				while(true)
				{
					mutex.lock();
					if(itr == myData.end())
					{
						mutex.unlock();
						break;
					}
					auto& entityID = itr.first;
					auto& data = itr.second;
					auto& prefabData = myPrefabData[0];
					itr++;
					mutex.unlock();
					aFunction(entityID, data, prefabData);
				}
			});
			threads.push_back(thread);
		}
		for (auto& it : threads)
			if(it->joinable())
				it->join();
		for (auto& it : threads)
			delete(it);
		threads.clear();
	}
}

template <class CData, class CPrefabData>
void CContainer<CData, CPrefabData>::InsertData(const EntityID anID, const CData& someData)
{
	myData[anID] = someData;
}

template <class CData, class CPrefabData>
void CContainer<CData, CPrefabData>::InsertPrefabData(const int anID, const CPrefabData& someData)
{
	myPrefabData[anID] = someData;
}

template <class CData, class CPrefabData>
void CContainer<CData, CPrefabData>::RemoveData(const EntityID anID)
{
	auto itr = myData.find(anID);
	if (itr != myData.end())
		myData.erase(anID);
}

template <class CData, class CPrefabData>
CData& CContainer<CData, CPrefabData>::GetData(const EntityID anID)
{
	return myData[anID];
}

template <class CData, class CPrefabData>
CPrefabData& CContainer<CData, CPrefabData>::GetPrefabData(const int anID)
{
	return myPrefabData[anID];
}