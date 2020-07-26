#include "PrefabData.h"

PrefabData::PrefabData()
{
}

PrefabData::PrefabData(const PrefabData& anOther)
{
	myStoredMap = anOther.myStoredMap;
}

void PrefabData::Serialize(rapidjson::Writer<rapidjson::StringBuffer>& aBase)
{
	aBase.Key("Properties");
	aBase.StartArray();
	for (auto& it : myStoredMap)
	{
		aBase.StartObject();
		aBase.Key("Name");
		aBase.String(it.first.c_str());

		auto hash = it.second->GetHash();
		if (hash == std::type_index(typeid(int)))
		{
			int type = 0;
			Serialize::Serialize("Type", type, aBase);
			auto val = *reinterpret_cast<int*>(it.second->GetData());
			Serialize::Serialize("Value", val, aBase);
		}
		else if (hash == std::type_index(typeid(float)))
		{
			int type = 1;
			Serialize::Serialize("Type", type, aBase);
			auto val = *reinterpret_cast<float*>(it.second->GetData());
			Serialize::Serialize("Value", val, aBase);
		}
		else if (hash == std::type_index(typeid(bool)))
		{
			int type = 2;
			Serialize::Serialize("Type", type, aBase);
			auto val = *reinterpret_cast<bool*>(it.second->GetData());
			Serialize::Serialize("Value", val, aBase);
		}
		else if (hash == std::type_index(typeid(std::string)))
		{
			int type = 3;
			Serialize::Serialize("Type", type, aBase);
			auto val = *reinterpret_cast<std::string*>(it.second->GetData());
			Serialize::Serialize("Value", val, aBase);
		}
		else if (hash == std::type_index(typeid(CommonUtilities::Vector2<float>)))
		{
			int type = 4;
			Serialize::Serialize("Type", type, aBase);
			auto val = *reinterpret_cast<CommonUtilities::Vector2<float>*>(it.second->GetData());
			Serialize::Serialize("Value", val, aBase);
		}
		else if (hash == std::type_index(typeid(CommonUtilities::Vector3<float>)))
		{
			int type = 5;
			Serialize::Serialize("Type", type, aBase);
			auto val = *reinterpret_cast<CommonUtilities::Vector3<float>*>(it.second->GetData());
			Serialize::Serialize("Value", val, aBase);
		}
		else if (hash == std::type_index(typeid(CommonUtilities::Vector4<float>)))
		{
			int type = 6;
			Serialize::Serialize("Type", type, aBase);
			auto val = *reinterpret_cast<CommonUtilities::Vector4<float>*>(it.second->GetData());
			Serialize::Serialize("Value", val, aBase);
		}
		else if (hash == std::type_index(typeid(CommonUtilities::Matrix4x4<float>)))
		{
			int type = 7;
			Serialize::Serialize("Type", type, aBase);
			auto val = *reinterpret_cast<CommonUtilities::Matrix4x4<float>*>(it.second->GetData());
			Serialize::Serialize("Value", val, aBase);
		}
		aBase.EndObject();
	}
	aBase.EndArray();
}

void PrefabData::Deserialize(const rapidjson::Value::Object& aContainer)
{
	if (!aContainer.HasMember("Properties"))
		return;
	if (!aContainer["Properties"].IsArray())
		return;
	auto arr = aContainer["Properties"].GetArray();
	for(auto& it : arr)
	{
		std::string name;
		int type = -1;
		Deserialize::Deserialize(it.GetObject(), "Name", name);
		Deserialize::Deserialize(it.GetObject(), "Type", type);
		switch(type)
		{
		case 0: // Int
			{
				int val = 0;
				Deserialize::Deserialize(it.GetObject(), "Value", val);
				auto stored = GetStored<int>(name);
				stored->Set(val);
			}
			break;
		case 1: // Float
			{
				float val = 0;
				Deserialize::Deserialize(it.GetObject(), "Value", val);
				auto stored = GetStored<int>(name);
				stored->Set(val);
			}
			break;
		case 2: // Bool
			{
				bool val = false;
				Deserialize::Deserialize(it.GetObject(), "Value", val);
				auto stored = GetStored<int>(name);
				stored->Set(val);
			}
			break;
		case 3: // String
			{
				std::string val;
				Deserialize::Deserialize(it.GetObject(), "Value", val);
				auto stored = GetStored<std::string>(name);
				stored->Set(val);
			}
			break;
		case 4: // Vec2
			{
				CommonUtilities::Vector2<float> val;
				Deserialize::Deserialize(it.GetObject(), "Value", val);
				auto stored = GetStored<CommonUtilities::Vector2<float>>(name);
				stored->Set(val);
			}
			break;
		case 5: // Vec3
			{
				CommonUtilities::Vector3<float> val;
				Deserialize::Deserialize(it.GetObject(), "Value", val);
				auto stored = GetStored<CommonUtilities::Vector3<float>>(name);
				stored->Set(val);
			}
			break;
		case 6: // Vec4
			{
				CommonUtilities::Vector4<float> val;
				Deserialize::Deserialize(it.GetObject(), "Value", val);
				auto stored = GetStored<CommonUtilities::Vector4<float>>(name);
				stored->Set(val);
			}
			break;
		case 7: // Matrix4x4
			{
				CommonUtilities::Matrix4x4<float> val;
				Deserialize::Deserialize(it.GetObject(), "Value", val);
				auto stored = GetStored<CommonUtilities::Matrix4x4<float>>(name);
				stored->Set(val);
			}
			break;
		default:
			Debug::Warning << "Unknown type of prefab property " << name << ", type: " << type << std::endl;
			break;
		}
	}
}

void PrefabData::Clear()
{
	for (auto& it : myStoredMap)
		delete(it.second);
	myStoredMap.clear();
}

