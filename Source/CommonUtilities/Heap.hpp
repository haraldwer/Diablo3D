#pragma once
#include "GrowingArray.h"

namespace CommonUtilities
{
	template <class T>
	class Heap
	{
	public:
		Heap();
		int Size() const;
		void Enqueue(const T& aElement);
		const T& GetTop() const;
		T Dequeue();
		
	private:
		static int GetRightChild(const int aIndex);
		static int GetLeftChild(const int aIndex);
		static int GetParent(const int aIndex);
		void Switch(int aNew, int anOld);
		std::vector<T> data;
	};

	template <class T>
	Heap<T>::Heap()
	{
		//data.Init(1, true);
	}

	template <class T>
	int Heap<T>::Size() const
	{
		return data.size();
	}

	template <class T>
	void Heap<T>::Enqueue(const T& aElement)
	{
		int index = data.size();
		data.push_back(aElement);
		while(index > 0)
		{
			int parentIndex = GetParent(index);
			if (data[parentIndex] < aElement)
			{
				Switch(parentIndex, index);
				index = parentIndex;
			}
			else
				break;
		}
		int asd = 0;
	}

	template <class T>
	const T& Heap<T>::GetTop() const
	{
		return data[0];
	}

	template <class T>
	T Heap<T>::Dequeue()
	{
		T temp = data[0];
		data[0] = data[data.size() - 1];
		data.erase(data.begin() + data.size() - 1);
		//data.RemoveAtIndex(data.Size()-1);
		int index = 0;
		while(true)
		{
			int right = GetRightChild(index);
			int left = GetLeftChild(index);
			int bigger = right;
			int smaller = left;
			
			if (right >= data.size() && left >= data.size())
				break;

			if(right >= data.size())
			{
				bigger = left;
				smaller = left;
			}
			else if (left >= data.size())
			{
				bigger = right;
				smaller = right;
			}
			else
			{
				if(data[right] < data[left])
				{
					bigger = left;
					smaller = right;
				}
			}
			
			if (data[index] < data[bigger])
			{
				Switch(bigger, index);
				index = bigger;
			}
			else if (data[index] < data[smaller])
			{
				Switch(smaller, index);
				index = smaller;
			}
			else
				break;
		}
		return temp;
	}

	template <class T>
	int Heap<T>::GetRightChild(const int aIndex)
	{
		return 2 * aIndex + 2;
	}

	template <class T>
	int Heap<T>::GetLeftChild(const int aIndex)
	{
		return 2 * aIndex + 1;
	}

	template <class T>
	int Heap<T>::GetParent(const int aIndex)
	{
		return (aIndex - 1) / 2;
	}

	template <class T>
	void Heap<T>::Switch(int aNew, int anOld)
	{
		T temp = data[anOld];
		data[anOld] = data[aNew];
		data[aNew] = temp;
	}
}
