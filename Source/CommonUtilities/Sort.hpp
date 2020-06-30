
#include <vector>

namespace CommonUtilities
{
	template <class T>
	void swap(T* xp, T* yp)
	{
		T temp = *xp;
		*xp = *yp;
		*yp = temp;
	}
	
	template <class T>
	void SelectionSort(std::vector<T>& aVector)
	{
		for (int i = 0; i < aVector.size() - 1; i++)
		{
			int min_idx = i;
			for (int j = i + 1; j < aVector.size(); j++)
				if (aVector[j] < aVector[min_idx])
					min_idx = j;
			swap(&aVector[min_idx], &aVector[i]);
		}
	}

	template <class T>
	void BubbleSort(std::vector<T>& aVector)
	{
		int i, j;
		for (i = 0; i < aVector.size() - 1; i++)
			for (j = 0; j < aVector.size() - i - 1; j++)
				if (aVector[j] > aVector[j + 1])
					swap(&aVector[j], &aVector[j + 1]);
	}

	template <class T>
	int QuickSortPartition(std::vector<T>& aVector, int low, int high)
	{
		T pivot = aVector[high]; // pivot  
		int i = (low - 1); // Index of smaller element  

		for (int j = low; j <= high - 1; j++)
		{
			// If current element is smaller than the pivot  
			if (aVector[j] < pivot)
			{
				i++; // increment index of smaller element  
				swap(&aVector[i], &aVector[j]);
			}
		}
		swap(&aVector[i + 1], &aVector[high]);
		return (i + 1);
	}
	
	template <class T>
	void QuickSortInternal(std::vector<T>& aVector, int aLow, int aHigh)
	{
		if (aLow < aHigh)
		{
			int pi = QuickSortPartition(aVector, aLow, aHigh);
			QuickSortInternal(aVector, aLow, pi - 1);
			QuickSortInternal(aVector, pi + 1, aHigh);
		}
	}

	template <class T>
	void QuickSort(std::vector<T>& aVector)
	{
		QuickSortInternal(aVector, 0, aVector.size() - 1);
	}
	
	template <class T>
	void MergeSortMerge(std::vector<T>& aVector, const int aLeft, const int aMiddle, const int aRight)
	{
		int i, j, k;
		int n1 = aMiddle - aLeft + 1;
		int n2 = aRight - aMiddle;

		/* create temp arrays */
		//T L[n1], R[n2];
		std::vector<T> L;
		std::vector<T> R;
		L.reserve(n1);
		R.reserve(n2);
		
		/* Copy data to temp arrays L[] and R[] */
		for (i = 0; i < n1; i++)
			L.push_back(aVector[aLeft + i]);
		for (j = 0; j < n2; j++)
			R.push_back(aVector[aMiddle + 1 + j]);

		i = 0; // Initial index of first subarray 
		j = 0; // Initial index of second subarray 
		k = aLeft; // Initial index of merged subarray 
		while (i < n1 && j < n2)
		{
			if (L[i] <= R[j])
			{
				aVector[k] = L[i];
				i++;
			}
			else
			{
				aVector[k] = R[j];
				j++;
			}
			k++;
		}
		
		while (i < n1)
		{
			aVector[k] = L[i];
			i++;
			k++;
		}
		
		while (j < n2)
		{
			aVector[k] = R[j];
			j++;
			k++;
		}
	}

	template <class T>
	void MergeSortInternal(std::vector<T>& aVector, int aLeft, int aRight)
	{
		if (aLeft < aRight)
		{
			int middle = aLeft + (aRight - aLeft) / 2;
			MergeSortInternal(aVector, aLeft, middle);
			MergeSortInternal(aVector, middle + 1, aRight);
			MergeSortMerge(aVector, aLeft, middle, aRight);
		}
	}
	
	template <class T>
	void MergeSort(std::vector<T>& aVector)
	{
		MergeSortInternal(aVector, 0, aVector.size() - 1);
	}
}
