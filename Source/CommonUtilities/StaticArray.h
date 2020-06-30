#pragma once
#include <initializer_list>
#include <cassert>

namespace CommonUtilities
{
	template <typename T, int size>
	class StaticArray
	{
	public:
		StaticArray() = default;
		StaticArray(const StaticArray& aStaticArray);
		StaticArray(const std::initializer_list<T>& aInitList);
		~StaticArray() = default;
		StaticArray& operator=(const StaticArray& aStaticArray);
		inline const T& operator[](const int aIndex) const;
		inline T& operator[](const int aIndex);
		inline void Insert(const int aIndex, const T& aObject);
	private:
		T data[size];
	};

	template <typename T, int size>
	StaticArray<T, size>::StaticArray(const StaticArray& aStaticArray)
	{
		(*this) = aStaticArray;
	}

	template <typename T, int size>
	StaticArray<T, size>::StaticArray(const std::initializer_list<T>& aInitList)
	{
		// Initializing from initList
		assert(aInitList.size() <= size && "Size of init list is bigger than array");
		int count = 0;
		for (auto& element : aInitList)
		{
			if(count < size)
			{
				data[count] = element;
			}
			++count;
		}
		if(count < size)
		{
			for(int i = count; i < size; i++)
			{
				data[i] = T();
			}
		}
	}

	template <typename T, int size>
	StaticArray<T, size>& StaticArray<T, size>::operator=(const StaticArray& aStaticArray)
	{
		for (int i = 0; i < size; i++)
		{
			data[i] = aStaticArray.data[i];
		}
		return (*this);
	}

	template <typename T, int size>
	const T& StaticArray<T, size>::operator[](const int aIndex) const
	{
		assert(aIndex >= 0 && aIndex <= size && "Index is outside of bounds");
		return data[aIndex];
	}

	template <typename T, int size>
	T& StaticArray<T, size>::operator[](const int aIndex)
	{
		assert(aIndex >= 0 && aIndex <= size && "Index is outside of bounds");
		return data[aIndex];
	}

	template <typename T, int size>
	void StaticArray<T, size>::Insert(const int aIndex, const T& aObject)
	{
		assert(aIndex >= 0 && aIndex <= size && "Index is outside of bounds");
		for(int i = size-1; i >= 0; --i)
		{
			if(i >= aIndex)
			{
				if(i < size-1) // Can only push up if there is something above
				{
					data[i + 1] = data[i]; // Replace
				}
				if (i == aIndex)
				{
					data[aIndex] = aObject;
					return;
				}
			}
			else
			{
				assert("Something went wrong");
			}
		}
		assert("Something went wrong");
	}
}


