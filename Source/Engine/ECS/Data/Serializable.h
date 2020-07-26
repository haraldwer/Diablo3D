#pragma once
#include "SerializableBase.h"
#include "Stored.h"
#include "PrefabData.h"

template <class T>
class Serializable : public SerializableBase
{
public:
	Serializable(const std::string& aName, const EditorControls aEditorControls = EditorControls::DEFAULT);
	Serializable(const Serializable& anOther);
	void Serialize(rapidjson::Writer<rapidjson::StringBuffer>& aBase) override;
	void Deserialize(const rapidjson::Value::Object& aContainer) override;
	void SetStored(PrefabData& somePrefabData) override;
	std::type_index GetType() override;
	
	void Set(const T& someData);
	T Get();
	void Apply();
	bool IsOverride() const;
	
private:
	T myData;
	bool myOverride;
	Stored<T>* myStored;
};

template <class T>
Serializable<T>::Serializable(const std::string& aName, const EditorControls aEditorControls) : SerializableBase(aName, aEditorControls), myOverride(false), myStored(nullptr)
{
}

template <class T>
Serializable<T>::Serializable(const Serializable& anOther) : SerializableBase(anOther), myData(anOther.myData), myOverride(anOther.myOverride), myStored(anOther.myStored)
{
}

template <class T>
void Serializable<T>::Serialize(rapidjson::Writer<rapidjson::StringBuffer>& aBase)
{
	if(myOverride)
		Serialize::Serialize(GetName().c_str(), myData, aBase);
}

template <class T>
void Serializable<T>::Deserialize(const rapidjson::Value::Object& aContainer)
{
	if (Deserialize::Deserialize(aContainer, GetName().c_str(), myData, false))
		myOverride = true;
}

template <class T>
void Serializable<T>::SetStored(PrefabData& somePrefabData)
{
	myStored = somePrefabData.GetStored<T>(GetName());
}

template <class T>
std::type_index Serializable<T>::GetType()
{
	return std::type_index(typeid(T));
}

template <class T>
void Serializable<T>::Set(const T& someData)
{
	myData = someData;
	myOverride = true;
}

template <class T>
T Serializable<T>::Get()
{
	if(myOverride || !myStored)
		return myData;

	return myStored->Get(); // This should be the data from the prefab
}

template <class T>
void Serializable<T>::Apply()
{
	// Apply override to prefab
	myOverride = false;
	if (!myStored)
		return;
	myStored->Set(myData);
}

template <class T>
bool Serializable<T>::IsOverride() const
{
	return myOverride;
}
