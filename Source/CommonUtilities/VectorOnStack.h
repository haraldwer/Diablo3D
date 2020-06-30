#pragma once
#include <initializer_list>
#include <cassert>

namespace CommonUtilities
{
	template <typename Type, int size, typename CountType = unsigned short, bool UseSafeModeFlag = true>
	class VectorOnStack
	{
	public:
		VectorOnStack();
		VectorOnStack(const VectorOnStack& aVectorOnStack);
		VectorOnStack(const std::initializer_list<Type>& aInitList);
		~VectorOnStack();
		VectorOnStack& operator=(const VectorOnStack& aVectorOnStack);
		inline const Type& operator[](const CountType aIndex) const;
		inline Type& operator[](const CountType aIndex);
		inline void Add(const Type& aObject);
		inline void Insert(const CountType aIndex, const Type& aObject);
		inline void RemoveCyclic(const Type& aObject);
		inline void RemoveCyclicAtIndex(const CountType aIndex);
		inline void Clear();
		inline CountType Size() const;
	private:
		Type data[size];
		CountType count;
	};

	template <typename Type, int size, typename CountType, bool UseSafeModeFlag>
	VectorOnStack<Type, size, CountType, UseSafeModeFlag>::VectorOnStack()
	{
		count = 0;
	}

	template <typename Type, int size, typename CountType, bool UseSafeModeFlag>
	VectorOnStack<Type, size, CountType, UseSafeModeFlag>::VectorOnStack(const VectorOnStack& aVectorOnStack)
	{
		(*this) = aVectorOnStack;
	}

	template <typename Type, int size, typename CountType, bool UseSafeModeFlag>
	VectorOnStack<Type, size, CountType, UseSafeModeFlag>::VectorOnStack(const std::initializer_list<Type>& aInitList)
	{
		count = 0;
		for (auto& element : aInitList)
		{
			Add(element);
		}
	}

	template <typename Type, int size, typename CountType, bool UseSafeModeFlag>
	VectorOnStack<Type, size, CountType, UseSafeModeFlag>::~VectorOnStack()
	{
		count = 0;
	}

	template <typename Type, int size, typename CountType, bool UseSafeModeFlag>
	VectorOnStack<Type, size, CountType, UseSafeModeFlag>& VectorOnStack<Type, size, CountType, UseSafeModeFlag>::operator=(const VectorOnStack& aVectorOnStack)
	{
		if (UseSafeModeFlag)
		{
			count = 0;
			for (int i = 0; i < aVectorOnStack.count; i++)
			{
				Add(aVectorOnStack[i]);
			}
		}
		else
		{
			count = aVectorOnStack.count;
			memcpy(data, aVectorOnStack.data, sizeof(Type) * size);
		}
		return (*this);
	}

	template <typename Type, int size, typename CountType, bool UseSafeModeFlag>
	const Type& VectorOnStack<Type, size, CountType, UseSafeModeFlag>::operator[](const CountType aIndex) const
	{
		assert(aIndex >= 0 && aIndex < count && "Index is outside of bounds");
		return data[aIndex];
	}

	template <typename Type, int size, typename CountType, bool UseSafeModeFlag>
	Type& VectorOnStack<Type, size, CountType, UseSafeModeFlag>::operator[](const CountType aIndex)
	{
		assert(aIndex >= 0 && aIndex < count && "Index is outside of bounds");
		return data[aIndex];
	}

	template <typename Type, int size, typename CountType, bool UseSafeModeFlag>
	void VectorOnStack<Type, size, CountType, UseSafeModeFlag>::Add(const Type& aObject)
	{
		assert(count < size && "Vector is full");
		data[count] = aObject;
		++count;
	}

	template <typename Type, int size, typename CountType, bool UseSafeModeFlag>
	void VectorOnStack<Type, size, CountType, UseSafeModeFlag>::Insert(const CountType aIndex, const Type& aObject)
	{
		assert(data != nullptr && "Array needs to be initialized");
		assert(aIndex >= 0 && aIndex <= count && "Index is outside of bounds");
		assert(count <= size && "Can't insert into a full vector");
		for (CountType i = count - 1; i >= aIndex && i > 0 && i < size; --i)
		{
			data[i + 1] = data[i];
		}
		data[aIndex] = aObject;
		++count;
	}

	template <typename Type, int size, typename CountType, bool UseSafeModeFlag>
	void VectorOnStack<Type, size, CountType, UseSafeModeFlag>::RemoveCyclic(const Type& aObject)
	{
		for(int i = 0; i < count; i++)
		{
			if(data[i] == aObject)
			{
				data[i] = data[count - 1];
				--count;
				return;
			}
		}
	}

	template <typename Type, int size, typename CountType, bool UseSafeModeFlag>
	void VectorOnStack<Type, size, CountType, UseSafeModeFlag>::RemoveCyclicAtIndex(const CountType aIndex)
	{
		assert(aIndex >= 0 && aIndex < count && "Index is outside of bounds");
		data[aIndex] = data[count - 1];
		--count;
	}

	template <typename Type, int size, typename CountType, bool UseSafeModeFlag>
	void VectorOnStack<Type, size, CountType, UseSafeModeFlag>::Clear()
	{
		count = 0;
	}

	template <typename Type, int size, typename CountType, bool UseSafeModeFlag>
	CountType VectorOnStack<Type, size, CountType, UseSafeModeFlag>::Size() const
	{
		return static_cast<CountType>(count);
	}
}
