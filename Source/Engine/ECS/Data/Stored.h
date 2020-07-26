#pragma once
#include "StoredBase.h"

template <class T>
class Stored : public StoredBase
{
public:
	void Set(T aValue);
	T Get();
	void* GetData() override;
	std::type_index GetHash() override;
private:
	T myData; 
};

template <class T>
void Stored<T>::Set(T aValue)
{
	myData = aValue;
}

template <class T>
T Stored<T>::Get()
{
	return myData;
}

template <class T>
void* Stored<T>::GetData()
{
	return &myData;
}

template <class T>
std::type_index Stored<T>::GetHash()
{
	return std::type_index(typeid(T));
}
