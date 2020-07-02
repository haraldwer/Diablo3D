#include "EngineLoader.h"


EngineLoader::EngineLoader(D3DSystem& system, const char* dllPath)
{
	mySystem = &system;
	myDLLPath = dllPath;
	Create();
}

EngineLoader::~EngineLoader()
{
	Destroy();
}

void EngineLoader::Update(bool run)
{
	if (myUpdate)
		myUpdate(&run);
}

void EngineLoader::Init()
{
	if(myInit)
		myInit();
}

void EngineLoader::Reload()
{
	Create();
}

Debug::Logger* EngineLoader::GetLog()
{
	if(myEngineDLL != NULL)
	{
		const RetDataFunc get = Load<RetDataFunc>("GetLog");
		if (get) 
			return reinterpret_cast<Debug::Logger*>(get());
	}
	return nullptr;
}

void EngineLoader::Create()
{
	myEngineDLL = LoadLibraryA(myDLLPath);
	if (myEngineDLL == NULL)
	{
		Debug::Log << "Unable to load engine DLL" << std::endl;
		return;
	}

	const Func create = Load<Func>("Create");
	if (create) create();

	const DataFunc setRenderer = Load<DataFunc>("SetRenderer");
	if(setRenderer && mySystem)
		setRenderer(mySystem);

	myInit = Load<Func>("Init");
	myUpdate = Load<DataFunc>("Update");
}

void EngineLoader::Destroy()
{
	if (myEngineDLL == NULL)
		return;
	Func destroy = Load<Func>("Destroy");
	if (destroy) 
		destroy();
	myInit = nullptr;
	myUpdate = nullptr;
	FreeLibrary(myEngineDLL);
	myEngineDLL = NULL;
}
