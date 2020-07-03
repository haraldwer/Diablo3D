#pragma once
#include <typeindex>
#include <unordered_map>
#include "../CommonUtilities/Log.h"

class ServiceBase
{
public:
	virtual ~ServiceBase() = default;
	virtual std::string GetName() = 0;
	virtual void Editor() { }
};

#pragma once
class ServiceLocator
{
	template <class T>
	friend class Service;
	friend class Engine;
public:
	static ServiceLocator& Instance();
	template <class T> 
	T& GetService();
	std::unordered_map<std::type_index, ServiceBase*>& GetServices();
	
private:
	ServiceLocator();
	~ServiceLocator();
	static ServiceLocator* instance;
	std::unordered_map<std::type_index, ServiceBase*> myServices;
};

template <class T>
T& ServiceLocator::GetService()
{
	auto type = std::type_index(typeid(T));
	return *(reinterpret_cast<T*>(myServices[type]));
}

template <class T>
class Service : public ServiceBase
{
	friend class ServiceLocator;
public:
	Service();
	virtual ~Service();
};

template <class T>
Service<T>::Service()
{
	auto type = std::type_index(typeid(T));
	auto& services = ServiceLocator::Instance().myServices;
	auto itr = services.find(type);
	if (itr != services.end())
	{
		Debug::Error << "This service already exists. " << std::endl;
		return;
	}
	ServiceLocator::Instance().myServices[type] = this;
}

template <class T>
Service<T>::~Service()
{
	auto type = std::type_index(typeid(T));
	ServiceLocator::Instance().myServices[type] = nullptr;
}