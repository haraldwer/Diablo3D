#pragma once
#include <unordered_map>
#include <functional>
#include <thread>
#include <mutex>
#include <atomic>
#include "../../Entity/Entity.h"
#include "PrefabData.h"

template <class Data>
class Container
{
	struct DataSlot
	{
		Data data;
		bool enabled;
	};
	
public:
	Data& Insert(Entity& anEntity);
	void InsertPrefab(const PrefabID anID, const PrefabData& someData);
	void Remove(const EntityID anID);
	bool HasData(const EntityID anID);
	Data& GetData(const EntityID anID);
	PrefabData& GetPrefab(const PrefabID anID);
	bool SetEnabled(const EntityID anID, const bool aEnabled);
	
	// Iterates through all components and performs function on their data. Returns false if it failed
	bool Perform(EntityID anEntityID, std::function<bool(const EntityID anID, Data& someData)> aFunction);
	bool Iterate(std::function<bool(const EntityID anEntityID, Data& someData)> aFunction);
	bool IterateAll(std::function<bool(const EntityID anEntityID, Data& someData)> aFunction);
	bool IterateThread(std::function<bool(const EntityID anEntityID, Data& someData)> aFunction);

	// Mutex lockable
	void Lock();
	void Unlock();
	size_t GetEntityCount();

private:
	std::mutex myMutex;
	std::unordered_map<EntityID, DataSlot> myData;
	std::unordered_map<EntityID, PrefabID> myPrefabIDs;
	std::unordered_map<PrefabID, PrefabData> myPrefabData; 
};

template <class Data>
bool Container<Data>::Perform(EntityID anEntityID, std::function<bool(const EntityID anID, Data& someData)> aFunction)
{
	auto find = myData.find(anEntityID);
	if (find == myData.end())
		return false;
	return aFunction(anEntityID, find->second.data); 
}

template <class Data>
bool Container<Data>::Iterate(std::function<bool(const EntityID anEntityID, Data& someData)> aFunction)
{
	for (auto& it : myData)
		if(it.second.enabled)
			if (!aFunction(it.first, it.second.data))
				return false;
	return true;
}

template <class Data>
bool Container<Data>::IterateAll(std::function<bool(const EntityID anEntityID, Data& someData)> aFunction)
{
	for (auto& it : myData)
		if (!aFunction(it.first, it.second.data))
			return false;
	return true;
}

template <class Data>
bool Container<Data>::IterateThread(std::function<bool(const EntityID anEntityID, Data& someData)> aFunction)
{
	//may return 0 when not able to detect
	const auto processor_count = std::thread::hardware_concurrency();
	if (processor_count == 0)
		return Iterate(aFunction);
	
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
				auto& entityID = itr->first;
				auto& data = itr->second.data;
				itr++;
				mutex.unlock();
				if (!aFunction(entityID, data))
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

template <class Data>
Data& Container<Data>::Insert(Entity& anEntity)
{
	const EntityID eID = anEntity.GetID();
	const PrefabID pID = anEntity.GetPrefabID();
	myPrefabIDs[eID] = pID;
	myData[eID].data.SetEntity(&anEntity);
	myData[eID].data.Create(myPrefabData[pID]);
	myData[eID].enabled = true;
	return myData[eID].data;
}

template <class Data>
void Container<Data>::InsertPrefab(const PrefabID anID, const PrefabData& someData)
{
	myPrefabData[anID] = someData;
}

template <class Data>
void Container<Data>::Remove(const EntityID anID)
{
	auto itr = myData.find(anID);
	if (itr != myData.end())
		myData.erase(anID);
}

template <class Data>
bool Container<Data>::HasData(const EntityID anID)
{
	return myData.find(anID) != myData.end();
}

template <class Data>
Data& Container<Data>::GetData(const EntityID anID)
{
	return myData[anID].data;
}

template <class Data>
PrefabData& Container<Data>::GetPrefab(const PrefabID anID)
{
	return myPrefabData[anID];
}

template <class Data>
bool Container<Data>::SetEnabled(const EntityID anID, const bool aEnabled)
{
	auto itr = myData.find(anID);
	if (itr == myData.end())
		return false;
	itr->second.enabled = aEnabled;
	return true;
}

template <class Data>
void Container<Data>::Lock()
{
	myMutex.lock();
}

template <class Data>
void Container<Data>::Unlock()
{
	myMutex.unlock();
}

template <class Data>
size_t Container<Data>::GetEntityCount()
{
	return myData.size();
}
