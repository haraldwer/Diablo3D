#include "ServiceLocator.h"
#include "../CommonUtilities/Log.h"

ServiceLocator* ServiceLocator::instance = nullptr;

ServiceLocator& ServiceLocator::Instance()
{
	return *instance;
}

std::unordered_map<std::type_index, ServiceBase*>& ServiceLocator::GetServices()
{
	return myServices;
}

ServiceLocator::ServiceLocator()
{
	instance = this;
	Debug::Log << "ServiceLocator created";
}

ServiceLocator::~ServiceLocator()
{
	instance = nullptr;
}
