#pragma once
#include "CSystemManager.h"

template <class T>
class CSystemLoader
{
public:
	CSystemLoader();
};

template <class T>
CSystemLoader<T>::CSystemLoader()
{
	T* instance = new T();
	//CSystemManager::mySystems.push_back(static_cast<CSystem*>(instance));
}
