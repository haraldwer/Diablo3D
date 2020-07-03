#pragma once
#include <unordered_map>
#include <functional>
#include <thread>
#include <mutex>
#include <atomic>
#include "../Entity/Entity.h"
#include "../CommonUtilities/Log.h"

template <class CData, class CPrefabData>
class CContainer
{
public:
	CData& Insert(const Entity& anEntity);
	void InsertPrefabData(const int anID, const CPrefabData& someData);
	void RemoveData(const EntityID anID);
	CData& GetData(const EntityID anID);
	CPrefabData& GetPrefabData(const int anID);
	
	// Iterates through all components and performs function on their data. Returns false if it failed
	bool Perform(EntityID anEntityID, std::function<bool(const EntityID anID, CData& someData, const CPrefabData& somePrefabData)> aFunction);
	bool Iterate(std::function<bool(const EntityID anEntityID, CData& someData, const CPrefabData& somePrefabData)> aFunction);
	bool IterateThread(std::function<bool(const EntityID anEntityID, CData& someData, const CPrefabData& somePrefabData)> aFunction);

	// Mutex lockable
	void Lock();
	void Unlock();
	size_t GetEntityCount();

private:
	std::mutex myMutex;
	std::unordered_map<EntityID, CData> myData;
	std::unordered_map<EntityID, PrefabID> myPrefabIDs;
	std::unordered_map<PrefabID, CPrefabData> myPrefabData;
};

template <class CData, class CPrefabData>
bool CContainer<CData, CPrefabData>::Perform(EntityID anEntityID,
	std::function<bool(const EntityID anID, CData& someData, const CPrefabData& somePrefabData)> aFunction)
{
	return aFunction(anEntityID, myData[anEntityID], myPrefabData[myPrefabIDs[anEntityID]]);
}

template <class CData, class CPrefabData>
bool CContainer<CData, CPrefabData>::Iterate(std::function<bool(const EntityID anEntityID, CData& someData, const CPrefabData& somePrefabData)> aFunction)
{
	for (auto& it : myData)
		if (!aFunction(it.first, it.second, myPrefabData[myPrefabIDs[it.first]]))
			return false;
	return true;
}

template <class CData, class CPrefabData>
bool CContainer<CData, CPrefabData>::IterateThread(std::function<bool(const EntityID anEntityID, CData& someData, const CPrefabData& somePrefabData)> aFunction)
{
	//may return 0 when not able to detect
	const auto processor_count = std::thread::hardware_concurrency();
	if (processor_count == 0)
		if (!Iterate(aFunction))
			return false;
	
	auto itr = myData.begin();
	std::mutex mutex;
	std::atomic<bool> run = true;
	std::vector<std::thread*> threads;
	for(int i = 0; i < processor_count; i++)
	{
		std::thread* thread = new std::thread([&]
		{
			while(true)
			{
				mutex.lock();
				if(itr == myData.end() || !run)
				{
					mutex.unlock();
					break;
				}
				auto& entityID = itr.first;
				auto& data = itr.second;
				auto& prefabData = myPrefabData[myPrefabIDs[itr->first]];
				itr++;
				mutex.unlock();
				if (!aFunction(entityID, data, prefabData))
					run = false;
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
	return run;
}


template <class CData, class CPrefabData>
CData& CContainer<CData, CPrefabData>::Insert(const Entity& anEntity)
{
	const EntityID eID = anEntity.GetID();
	const PrefabID pID = anEntity.GetPrefabID();
	myPrefabIDs[eID] = pID;
	CData data = CData();
	myData[eID] = data;
	return myData[eID];
}

template <class CData, class CPrefabData>
void CContainer<CData, CPrefabData>::InsertPrefabData(const PrefabID anID, const CPrefabData& someData)
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
CPrefabData& CContainer<CData, CPrefabData>::GetPrefabData(const PrefabID anID)
{
	return myPrefabData[anID];
}

template <class CData, class CPrefabData>
void CContainer<CData, CPrefabData>::Lock()
{
	myMutex.lock();
}

template <class CData, class CPrefabData>
void CContainer<CData, CPrefabData>::Unlock()
{
	myMutex.unlock();
}

template <class CData, class CPrefabData>
size_t CContainer<CData, CPrefabData>::GetEntityCount()
{
	return myData.size();
}
