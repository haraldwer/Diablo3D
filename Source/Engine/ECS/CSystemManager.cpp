#include "CSystemManager.h"
#include "CSystem.h"
#include "CSystems.h"
#include "ImGui/imgui.h"

std::vector<CSystemBase*> CSystemManager::mySystems;
CSystemManager* CSystemManager::instance = nullptr;

CSystemManager::CSystemManager() : mySystemsClass(nullptr)
{
	instance = this;
}

CSystemManager::~CSystemManager()
{
	if (mySystemsClass)
	{
		delete(mySystemsClass);
		mySystemsClass = nullptr;
	}
	mySystems.clear();
	mySystemHashMap.clear();
	mySystemMap.clear();
}

void CSystemManager::Init()
{
	mySystemsClass = new CSystems();
	
	for (auto& it : mySystems)
	{
		mySystemMap[it->GetSystemName()] = it;
		mySystemHashMap[it->GetTypeIndex()] = it;
	}

	myPrefabManager.Init();
	
	for (auto& it : mySystems)
		it->Init();
}

void CSystemManager::Update()
{
	for (auto& it : mySystems)
		it->Update();
}

CSystemBase* CSystemManager::GetSystem(const std::string& aSystemName)
{
	const auto find = mySystemMap.find(aSystemName);
	if (find == mySystemMap.end())
	{
		Debug::Error << "Unable to find system with name " << aSystemName << std::endl;
		return nullptr;
	}
	return find->second;
}

CSystemBase* CSystemManager::GetSystem(const std::type_index& aSystemTypeIndex)
{
	const auto find = mySystemHashMap.find(aSystemTypeIndex);
	if (find == mySystemHashMap.end())
	{
		Debug::Error << "Unable to find system with hash " << aSystemTypeIndex.hash_code() << std::endl;
		return nullptr;
	}
	return find->second;
}

void CSystemManager::Editor()
{
	ImGui::Text("Component systems");
	ImGui::Separator();
	ImGui::BeginChild("CSystems");
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 1));
	for (auto& it : mySystemMap)
		ImGui::Text((it.first + ": " + std::to_string(it.second->GetEntityCount())).c_str());
	ImGui::PopStyleVar();
	ImGui::EndChild();
}

void CSystemManager::RemoveEntity(const EntityID anEntityID, const std::vector<std::type_index>& someSystemTypes)
{
	for(auto& it : someSystemTypes)
	{
		auto sys = GetSystem(it);
		if (sys)
			sys->RemoveEntity(anEntityID);
	}
}
