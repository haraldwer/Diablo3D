#include "Preferences.h"
#include "../RapidJSON/formatter.h"
#include <fstream>
#include "../RapidJSON/parser.h"

PreferenceBase::PreferenceBase(const std::string& aName) : myName(aName)
{
	Preferences::Add(this);
}

PreferenceBase::~PreferenceBase()
{
	Preferences::Remove(this);
}

std::string PreferenceBase::GetName() const
{
	return myName;
}

std::unordered_map<std::string, PreferenceBase*> Preferences::myPreferences;

void Preferences::Load()
{
	const std::string doc = Parse(path);
	if (doc.empty())
	{
		Debug::Error << "Unable to load editor preferences. File \"" << path << "\" might be missing." << std::endl;
		return;
	}
	rapidjson::Document json;
	json.Parse(doc.c_str());
	if (!json.IsObject())
	{
		Debug::Error << "Unable to load editor preferences. File \"" << path << "\" might be corrupted." << std::endl;
		return;
	}
	
	for(auto& it : myPreferences)
	{
		it.second->Load(json.GetObject());
	}
}

void Preferences::Save()
{
	rapidjson::StringBuffer s;
	rapidjson::Writer<rapidjson::StringBuffer> writer(s);
	writer.StartObject();
	for (auto& it : myPreferences)
	{
		it.second->Save(writer);
	}
	writer.EndObject();
	std::ofstream out(path);
	out.clear();
	out << Format(s.GetString());
	out.close();
}

void Preferences::Add(PreferenceBase* pref)
{
	myPreferences[pref->GetName()] = pref;
}

void Preferences::Remove(PreferenceBase* pref)
{
	if(myPreferences.find(pref->GetName()) != myPreferences.end())
		myPreferences.erase(pref->GetName());
}
