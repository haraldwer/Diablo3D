#pragma once
#include <string>
#include <typeindex>
#include <vector>
#include "../../Utility/JSONHelper.h"

class PrefabData;

enum class EditorControls
{
	DEFAULT,
	NONE,
	DRAG_INT,
	INPUT_INT,
	DRAG_FLOAT,
	INPUT_FLOAT,
	TEXT,
	RICH_TEXT,	// NOT IMPLEMENTED
	BUTTON,
	CHECKBOX,
	TEXTURE,	// NOT IMPLEMENTED
	MODEL,		// NOT IMPLEMENTED
	RESOURCE,	// NOT IMPLEMENTED
	COLOR,
};

class SerializableBase
{
public:
	SerializableBase(const std::string& aName, const EditorControls aEditorControls = EditorControls::DEFAULT);
	SerializableBase(const SerializableBase& anOther);
	std::string GetName() const;
	virtual ~SerializableBase() = default;
	virtual void Serialize(rapidjson::Writer<rapidjson::StringBuffer>& aBase) = 0;
	virtual void Deserialize(const rapidjson::Value::Object& aContainer) = 0;
	virtual void SetStored(PrefabData& aStored) = 0;
	virtual void Copy(SerializableBase* aBase) = 0;
	static std::vector<SerializableBase*> GetStored();
	virtual std::type_index GetType() = 0;
	EditorControls GetEditorControls() const;
	
private:
	const std::string myName;
	const EditorControls myEditorControls;
	
	static std::vector<SerializableBase*> stored;
};
