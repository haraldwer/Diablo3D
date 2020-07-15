#pragma once
#include <unordered_map>
#include <cassert>
#include <typeinfo>	

namespace CommonUtilities
{
	template<typename key, short MAXSIZE = 64>
	class OpaqueDictionary
	{
	public:
		OpaqueDictionary();
		~OpaqueDictionary();

		template<typename value>
		void				SetValue(const key& aKey, const value& aValue);

		template<typename value>
		const value& GetValue(const key& aKey) const;
		template<typename value>
		const std::type_info* GetType(const key& aKey) const;

		template<typename value>
		value& ChangeValue(const key& aKey);

		bool				HasValue(const key& aKey) const;

		void				Clear();

	private:
		std::unordered_map<key, void*>
			myDataPointers;
		char				myData[MAXSIZE];
		int					myCurrentIndex;

		std::unordered_map<key, const std::type_info*>
			myDataTypes;
	};

	template<typename key, short MAXSIZE>
	OpaqueDictionary<key, MAXSIZE>::OpaqueDictionary()
	{
		myCurrentIndex = 0;
	}

	template<typename key, short MAXSIZE>
	OpaqueDictionary<key, MAXSIZE>::~OpaqueDictionary()
	{

	}

	template<typename key, short MAXSIZE>
	bool OpaqueDictionary<key, MAXSIZE>::HasValue(const key& aKey) const
	{
		auto type = myDataPointers.find(aKey);
		return (type != myDataPointers.end());
	}


	template<typename key, short MAXSIZE>
	void OpaqueDictionary<key, MAXSIZE>::Clear()
	{
		myCurrentIndex = 0;
		myDataPointers.clear();
		myDataTypes.clear();
	}

	template<typename key, short MAXSIZE>
	template<typename value>
	void OpaqueDictionary<key, MAXSIZE>::SetValue(const key& aKey, const value& aValue)
	{
		assert(myCurrentIndex + sizeof(value) < MAXSIZE && "Error! Not enough memory in OpaqueDictionary!");

		auto it = myDataPointers.find(aKey);
#undef new
		if (it == myDataPointers.end())
		{
			myDataPointers[aKey] = new (&myData[myCurrentIndex]) value(aValue);

			myCurrentIndex += sizeof(value);
			myDataTypes[aKey] = &typeid(value);
		}
		else
		{
			assert(typeid(value) == *myDataTypes[aKey] && "OpaqueDictionary: Trying to set a value to a different type than it was created with!");
			*(reinterpret_cast<value*>((*it).second)) = aValue;
		}
	}

	template<typename key, short MAXSIZE>
	template<typename value>
	const value& OpaqueDictionary<key, MAXSIZE>::GetValue(const key& aKey) const
	{
		auto type = myDataTypes.find(aKey);
		assert(type != myDataTypes.end() && "Key not found in opaque dictionary!");
		assert(typeid(value) == *(*type).second && "Incorrect type!");

		auto it = myDataPointers.find(aKey);
		assert(it != myDataPointers.end() && "Key not found in opaque dictionary!");
		return *(reinterpret_cast<value*>((*it).second));
	}

	template <typename key, short MAXSIZE>
	template <typename value>
	const std::type_info* OpaqueDictionary<key, MAXSIZE>::GetType(const key& aKey) const
	{
		auto type = myDataTypes.find(aKey);
		assert(type != myDataTypes.end() && "Key not found in opaque dictionary!");
		return type->second;
	}

	template<typename key, short MAXSIZE>
	template<typename value>
	value& OpaqueDictionary<key, MAXSIZE>::ChangeValue(const key& aKey)
	{
		value& val = const_cast<value&>(GetValue<value>(aKey));
		return val;
	}
}
