#pragma once
#include <cassert>

namespace CommonUtilities
{
	template <typename Type, typename SizeType = unsigned short>
	class GrowingArray
	{
	public:
		GrowingArray();
		GrowingArray(SizeType aNrOfRecommendedItems, bool aUseSafeModeFlag = true);
		GrowingArray(const GrowingArray& aGrowingArray);
		GrowingArray(GrowingArray&& aGrowingArray);
		
		~GrowingArray();

		GrowingArray& operator=(const GrowingArray& aGrowingArray);
		GrowingArray& operator=(GrowingArray&& aGrowingArray);

		void Init(SizeType aNrOfRecommendedItems, bool aUseSafeModeFlag = true);
		inline Type& operator[](const SizeType aIndex);
		inline const Type& operator[](const SizeType aIndex) const;

		inline void Add(const Type& aObject);
		inline void Insert(const SizeType aIndex, const Type& aObject);
		inline void RemoveAtIndex( const SizeType aIndex );
		inline void RemoveCyclic(const Type& aObject);
		inline void RemoveCyclicAtIndex(SizeType aItemNumber);
		inline SizeType Find(const Type& aObject);

		static const SizeType FoundNone = -1;

		inline void RemoveAll();

		void Optimize();
		__forceinline SizeType Size() const;
		inline void Reserve(SizeType aNewSize);
		void Resize(SizeType aNewSize);
	private:
		Type* myData;
		SizeType myCount;
		SizeType mySize;
		bool myUseSafeMode;
	};

	template <typename Type, typename SizeType>
	GrowingArray<Type, SizeType>::GrowingArray()
	{
		myData = nullptr;
		myCount = 0;
		mySize = 0;
		myUseSafeMode = true;
	}

	template <typename Type, typename SizeType>
	GrowingArray<Type, SizeType>::GrowingArray(SizeType aNrOfRecommendedItems, const bool aUseSafeModeFlag)
	{
		myCount = 0;
		mySize = aNrOfRecommendedItems;
		myUseSafeMode = aUseSafeModeFlag;
		myData = nullptr;
	}

	template <typename Type, typename SizeType>
	GrowingArray<Type, SizeType>::GrowingArray(const GrowingArray& aGrowingArray)
	{
		myData = nullptr;
		operator=(aGrowingArray);
	}

	template <typename Type, typename SizeType>
	GrowingArray<Type, SizeType>::GrowingArray(GrowingArray&& aGrowingArray)
	{
		myData = nullptr;
		operator=(aGrowingArray);
	}

	template <typename Type, typename SizeType>
	GrowingArray<Type, SizeType>::~GrowingArray()
	{
		delete[](myData);
		myData = nullptr;
		mySize = 0;
		myCount = 0;
	}

	template <typename Type, typename SizeType>
	GrowingArray<Type, SizeType>& GrowingArray<Type, SizeType>::operator=(const GrowingArray& aGrowingArray)
	{
		delete[](myData);
		myData = new Type[aGrowingArray.mySize];
		mySize = aGrowingArray.mySize;
		myCount = aGrowingArray.myCount;
		myUseSafeMode = aGrowingArray.myUseSafeMode;
		if(myUseSafeMode)
		{
			// Copy every element
			for (SizeType i = 0; i < aGrowingArray.myCount; ++i)
			{
				myData[i] = aGrowingArray.myData[i];
			}
		}
		else
		{
			// Copy entire array
			memcpy(myData, aGrowingArray.myData, sizeof(Type) * mySize);
		}
		return (*this);
	}

	template <typename Type, typename SizeType>
	GrowingArray<Type, SizeType>& GrowingArray<Type, SizeType>::operator=(GrowingArray&& aGrowingArray)
	{
		operator=(aGrowingArray);
		return (*this);
	}

	template <typename Type, typename SizeType>
	void GrowingArray<Type, SizeType>::Init(const SizeType aNrOfRecommendedItems, const bool aUseSafeModeFlag)
	{
		delete[](myData);
		if(aNrOfRecommendedItems > 0)
		{
			myData = new Type[aNrOfRecommendedItems];
		}
		myCount = 0;
		mySize = aNrOfRecommendedItems;
		myUseSafeMode = aUseSafeModeFlag;
	}

	template <typename Type, typename SizeType>
	Type& GrowingArray<Type, SizeType>::operator[](const SizeType aIndex)
	{
		assert(myData != nullptr && "Array needs to be initialized");
		assert(aIndex >= 0 && aIndex < myCount && "Index outside of bounds");
		return myData[aIndex];
	}

	template <typename Type, typename SizeType>
	const Type& GrowingArray<Type, SizeType>::operator[](const SizeType aIndex) const
	{
		assert(myData != nullptr && "Array needs to be initialized");
		assert(aIndex >= 0 && aIndex < myCount && "Index outside of bounds");
		return myData[aIndex];
	}

	template <typename Type, typename SizeType>
	void GrowingArray<Type, SizeType>::Add(const Type& aObject)
	{
		assert(myData != nullptr && "Array needs to be initialized");
		
		if(myCount == mySize)
		{
			// Resize data
			Resize(mySize + 1);
		}
		myData[myCount] = aObject;
		++myCount;
	}

	template <typename Type, typename SizeType>
	void GrowingArray<Type, SizeType>::Insert(const SizeType aIndex, const Type& aObject)
	{
		assert(myData != nullptr && "Array needs to be initialized");
		if (myCount == mySize)
		{
			// Resize data
			Resize(mySize + 1);
		}
		for(SizeType i = myCount-1; i >= aIndex && i > 0 && i < mySize; --i)
		{
			myData[i + 1] = myData[i];
		}
		assert(aIndex >= 0 && aIndex <= myCount && "Index outside of bounds");
		myData[aIndex] = aObject;
		++myCount;
		return;
	}

	template <typename Type, typename SizeType>
	void GrowingArray<Type, SizeType>::RemoveAtIndex( const SizeType aIndex )
	{
		if ( myData == nullptr )
		{
			assert( "Array needs to be initialized" );
		}

		if ( aIndex < 0 && aIndex >= myCount )
		{
			assert( "Index outside of bounds" );
		}

		SizeType currentIndex = aIndex;
		while ( currentIndex + 1 < myCount )
		{
			myData[currentIndex] = myData[currentIndex + 1];
			currentIndex++;
		}
		myCount--;
	}

	template <typename Type, typename SizeType>
	void GrowingArray<Type, SizeType>::RemoveCyclic(const Type& aObject)
	{
		assert(myData != nullptr && "Array needs to be initialized");
		for (SizeType i = 0; i < myCount; i++)
		{
			if (myData[i] == aObject)
			{
				myData[i] = myData[myCount - 1];
				--myCount;
				return;
			}
		}
	}

	template <typename Type, typename SizeType>
	SizeType GrowingArray<Type, SizeType>::Find(const Type& aObject)
	{
		assert(myData != nullptr && "Array needs to be initialized");
		for (SizeType i = 0; i < myCount; ++i)
		{
			if (myData[i] == aObject)
			{
				return static_cast<SizeType>(i);
			}
		}
		return FoundNone;
	}

	template <typename Type, typename SizeType>
	void GrowingArray<Type, SizeType>::RemoveCyclicAtIndex(SizeType aItemNumber)
	{
		assert(myData != nullptr && "Array needs to be initialized");
		assert(aItemNumber >= 0 && aItemNumber < myCount && "Index outside of bounds");
		myData[aItemNumber] = myData[myCount - 1];
		--myCount;
	}

	template <typename Type, typename SizeType>
	void GrowingArray<Type, SizeType>::RemoveAll()
	{
		assert(myData != nullptr && "Array needs to be initialized");
		myCount = 0;
	}

	template <typename Type, typename SizeType>
	void GrowingArray<Type, SizeType>::Optimize()
	{
		Resize(myCount);
	}

	template <typename Type, typename SizeType>
	SizeType GrowingArray<Type, SizeType>::Size() const
	{
		assert(myData != nullptr && "Array needs to be initialized");
		return myCount;
	}

	template <typename Type, typename SizeType>
	void GrowingArray<Type, SizeType>::Resize(SizeType aNewSize)
	{
		assert(myData != nullptr && "Array needs to be initialized");
		assert(aNewSize >= myCount && "Can't shrink more than the current number of items");
		Type* newData = new Type[aNewSize];
		if (myUseSafeMode)
		{
			// Copy every element
			for (SizeType i = 0; i < myCount; ++i)
			{
				newData[i] = myData[i];
			}
		}
		else
		{
			// Copy entire array
			memcpy(newData, myData, sizeof(Type) * myCount);
		}
		delete[](myData);
		myData = newData;
		mySize = aNewSize;
		
	}

	template <typename Type, typename SizeType>
	void GrowingArray<Type, SizeType>::Reserve(SizeType aNewSize)
	{
		Resize(aNewSize);
	}
}
