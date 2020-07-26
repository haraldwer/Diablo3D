#include "SerializableBase.h"

std::vector<SerializableBase*> SerializableBase::stored;

SerializableBase::SerializableBase(const std::string& aName, const EditorControls aEditorControls) : myName(aName), myEditorControls(aEditorControls)
{
	stored.push_back(this);
}

SerializableBase::SerializableBase(const SerializableBase& anOther) : myName(anOther.myName), myEditorControls(anOther.myEditorControls)
{
	stored.push_back(this);
}

std::string SerializableBase::GetName() const
{
	return myName;
}

std::vector<SerializableBase*> SerializableBase::GetStored()
{
	auto s = stored;
	stored.clear();
	return s;
}

EditorControls SerializableBase::GetEditorControls() const
{
	return myEditorControls;
}
