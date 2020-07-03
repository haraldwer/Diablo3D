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
	return mySystemMap[aSystemName];
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
