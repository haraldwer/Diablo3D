#pragma once
#include <cstdint>
#include <vector>
#include <string>

namespace CommonUtilities
{
	uint32_t Hash(const std::string& aKey)
	{
		const uint8_t* aBuffer = reinterpret_cast<const uint8_t*>(aKey.c_str());
		const int count = aKey.size();

		const uint32_t FNVOffsetBasis = 2166136261U;
		const uint32_t FNVPrime = 16777619U;
		uint32_t val = FNVOffsetBasis;
		for (int i = 0; i < count; i++)
		{
			val ^= aBuffer[i];
			val *= FNVPrime;
		}
		return val;
	}
	
	template <class T>
	uint32_t Hash(T aKey)
	{
		const uint8_t* aBuffer = reinterpret_cast<const uint8_t*>(&aKey);
		const int count = sizeof(T); 
		
		const uint32_t FNVOffsetBasis = 2166136261U;
		const uint32_t FNVPrime = 16777619U;
		uint32_t val = FNVOffsetBasis;
		for(int i = 0; i < count; i++)
		{
			val ^= aBuffer[i];
			val *= FNVPrime;
		}
		return val;
	}
	
	template <class Key, class Value>
	class HashMap
	{
		enum class State
		{
			Empty,
			InUse,
			Removed
		};
		
		struct Entry
		{
			State state;
			Key key;
			Value value;
		};
		
	public:
		//Skapar en HashMap med plats för aCapacity element. Den behöver inte
		//kunna växa. Blir den full så blir den full.
		HashMap(int aCapacity);
		//Stoppar in aValue under nyckeln aKey. Om det fanns ett värde innan med
		//aKey som nyckel så skrivs det värdet över. Om det lyckas returneras
		//true. Om map:en är full så görs ingenting, och false returneras.
		bool Insert(const Key& aKey, const Value& aValue);
		//Plockar bort elementet med nyckeln aKey, om det finns. Om det finns
		//returneras true. Om det inte finns görs ingenting, och false
		//returneras.
		bool Remove(const Key& aKey);
		//Ger en pekare till värdet med nyckeln aKey, om det finns. Om det inte
		//finns returneras nullptr.
		const Value* Get(const Key& aKey) const;
		//Som ovan, men returnerar en icke-const-pekare.
		Value* Get(const Key& aKey);

	private:
		int FindIndex(const Key& aKey, const bool aFindEmpty = false);
		std::vector<Entry> myEntries;
		int myCapacity;
	};

	template <class Key, class Value>
	HashMap<Key, Value>::HashMap(int aCapacity)
	{
		myCapacity = aCapacity;
		myEntries.reserve(aCapacity);
		Entry e;
		e.state = State::Empty;
		for (int i = 0; i < aCapacity; i++)
			myEntries.push_back(e);
	}

	template <class Key, class Value>
	bool HashMap<Key, Value>::Insert(const Key& aKey, const Value& aValue)
	{
		const int index = FindIndex(aKey, true);
		if (index < 0 || index >= myCapacity)
			return false;
		myEntries[index].key = aKey;
		myEntries[index].value = aValue;
		myEntries[index].state = State::InUse;
		return true;
	}

	template <class Key, class Value>
	bool HashMap<Key, Value>::Remove(const Key& aKey)
	{
		const int index = FindIndex(aKey);
		if (index < 0 || index >= myCapacity)
			return false;
		myEntries[index].state = State::Removed;
		return true;
	}

	template <class Key, class Value>
	const Value* HashMap<Key, Value>::Get(const Key& aKey) const
	{
		const int index = FindIndex(aKey);
		if (index < 0 || index >= myCapacity)
			return nullptr;
		return &myEntries[index].value;
	}

	template <class Key, class Value>
	Value* HashMap<Key, Value>::Get(const Key& aKey)
	{
		const int index = FindIndex(aKey);
		if (index < 0 || index >= myCapacity)
			return nullptr;
		return &myEntries[index].value;
	}

	template <class Key, class Value>
	int HashMap<Key, Value>::FindIndex(const Key& aKey, const bool aFindEmpty)
	{
		if (myCapacity == 0)
			return -1;
		int index = Hash(aKey) % myCapacity;
		const int startIndex = index;
		do
		{
			if ((!aFindEmpty && myEntries[index].state == State::Empty) ||
				(myEntries[index].state == State::Removed && !(myEntries[index].key < aKey || aKey < myEntries[index].key)))
				break; // key doesnt exist

			if ((aFindEmpty && myEntries[index].state == State::Empty) || 
				(myEntries[index].state == State::InUse && !(myEntries[index].key < aKey || aKey < myEntries[index].key)))
				return index;
			
			++index;
			if (index >= myCapacity)
				index = 0;
		} while (index != startIndex);
		return -1;
	}
}
