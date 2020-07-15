#pragma once
#include <string>
#include "../Engine/Utility/JSONHelper.h"

class PreferenceBase
{
public:
	PreferenceBase(const std::string& aName);
	virtual ~PreferenceBase();
	std::string GetName() const;
	virtual void Load(const rapidjson::Value::Object& aContainer) = 0;
	virtual void Save(rapidjson::Writer<rapidjson::StringBuffer>& writer) = 0;
private:
	const std::string myName;
};

template <class T>
class Preference : public PreferenceBase
{
public:
	Preference(const std::string& aName, const T& aDefaultValue);
	T Get();
	void Set(const T& val);
	void Load(const rapidjson::Value::Object& aContainer) override;
	void Save(rapidjson::Writer<rapidjson::StringBuffer>& writer) override;
private:
	T myValue;
};

template <class T>
Preference<T>::Preference(const std::string& aName, const T& aDefaultValue) : PreferenceBase(aName), myValue(aDefaultValue)
{
}

template <class T>
T Preference<T>::Get()
{
	return myValue;
}

template <class T>
void Preference<T>::Set(const T& val)
{
	myValue = val;
}

#pragma region Load

inline void Preference<bool>::Load(const rapidjson::Value::Object& aContainer)
{
	myValue = Deserialize::Bool(aContainer, GetName().c_str(), myValue);
}

inline void Preference<float>::Load(const rapidjson::Value::Object& aContainer)
{
	myValue = Deserialize::Float(aContainer, GetName().c_str(), myValue);
}

inline void Preference<int>::Load(const rapidjson::Value::Object& aContainer)
{
	myValue = Deserialize::Int(aContainer, GetName().c_str(), myValue);
}

inline void Preference<std::string>::Load(const rapidjson::Value::Object& aContainer)
{
	myValue = Deserialize::String(aContainer, GetName().c_str(), myValue.c_str());
}

inline void Preference<CommonUtilities::Vector2<float>>::Load(const rapidjson::Value::Object& aContainer)
{
	myValue = Deserialize::Vector2(aContainer, GetName().c_str(), myValue);
}

inline void Preference<CommonUtilities::Vector3<float>>::Load(const rapidjson::Value::Object& aContainer)
{
	myValue = Deserialize::Vector3(aContainer, GetName().c_str(), myValue);
}

inline void Preference<CommonUtilities::Vector4<float>>::Load(const rapidjson::Value::Object& aContainer)
{
	myValue = Deserialize::Vector4(aContainer, GetName().c_str(), myValue);
}

#pragma endregion 

template <class T>
void Preference<T>::Load(const rapidjson::Value::Object& aContainer)
{
	Debug::Error << "Preference with name " << GetName() << " has unknown type" << std::endl;
}

template <class T>
void Preference<T>::Save(rapidjson::Writer<rapidjson::StringBuffer>& writer)
{
	Serialize::Serialize(GetName().c_str(), myValue, writer);
}

class Preferences
{
	friend class PreferenceBase;
public:
	void Load();
	void Save();
	
private:
	static void Add(PreferenceBase* pref);
	static void Remove(PreferenceBase* pref);
	inline static const char* path = "preferences.json";
	static std::unordered_map<std::string, PreferenceBase*> myPreferences;
};
