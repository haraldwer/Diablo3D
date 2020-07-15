#pragma once
#include "../CommonUtilities/Log.h"
#include "../RapidJSON/writer.h"
#include "../RapidJSON/stringbuffer.h"
#include "../RapidJSON/document.h"
#include <string>
#include "../CommonUtilities/Vector2.h"
#include "../CommonUtilities/Vector3.h"
#include "../CommonUtilities/Vector4.h"
#include "../CommonUtilities/Matrix4x4.h"

#define GET_VARIABLE_NAME(Variable) (#Variable)
#define SERIALIZE(variable) Serializer::Serialize(GET_VARIABLE_NAME(variable), variable, aBase)
#define DESERIALIZE(variable, type, defaultValue) variable = Deserialize::type(aBase, GET_VARIABLE_NAME(variable), defaultValue)

namespace Serialize
{
	inline void Serialize(const char* aVariableName, const float aValue, rapidjson::Writer<rapidjson::StringBuffer>& aBase)
	{
		aBase.Key(aVariableName);
		aBase.Double(aValue);
	}

	inline void Serialize(const char* aVariableName, const double aValue, rapidjson::Writer<rapidjson::StringBuffer>& aBase)
	{
		aBase.Key(aVariableName);
		aBase.Double(aValue);
	}

	inline std::string ReplaceAll(std::string str, const std::string& from, const std::string& to)
	{
		size_t start_pos = 0;
		while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
			str.replace(start_pos, from.length(), to);
			start_pos += to.length(); // Handles case where 'to' is a substring of 'from'
		}
		return str;
	}

	inline void Serialize(const char* aVariableName, const std::string& aValue, rapidjson::Writer<rapidjson::StringBuffer>& aBase)
	{
		aBase.Key(aVariableName);
		aBase.String(aValue.c_str());// ReplaceAll(aValue, "\n", "").c_str());
	}
	
	inline void Serialize(const char* aVariableName, const bool aValue, rapidjson::Writer<rapidjson::StringBuffer>& aBase)
	{
		aBase.Key(aVariableName);
		aBase.Bool(aValue);
	}

	inline void Serialize(const char* aVariableName, const int aValue, rapidjson::Writer<rapidjson::StringBuffer>& aBase)
	{
		aBase.Key(aVariableName);
		aBase.Int(aValue);
	}

	inline void Serialize(const char* aVariableName, const CommonUtilities::Vector2<float>& aValue, rapidjson::Writer<rapidjson::StringBuffer>& aBase)
	{
		aBase.Key(aVariableName);
		aBase.StartObject();
		aBase.Key("x");
		aBase.Double(aValue.x);
		aBase.Key("y");
		aBase.Double(aValue.y);
		aBase.EndObject();
	}

	inline void Serialize(const char* aVariableName, const CommonUtilities::Vector3<float>& aValue, rapidjson::Writer<rapidjson::StringBuffer>& aBase)
	{
		aBase.Key(aVariableName);
		aBase.StartObject();
		aBase.Key("x");
		aBase.Double(aValue.x);
		aBase.Key("y");
		aBase.Double(aValue.y);
		aBase.Key("z");
		aBase.Double(aValue.z);
		aBase.EndObject();
	}
	
	inline void Serialize(const char* aVariableName, const CommonUtilities::Vector4<float>& aValue, rapidjson::Writer<rapidjson::StringBuffer>& aBase)
	{
		aBase.Key(aVariableName);
		aBase.StartObject();
		aBase.Key("x");
		aBase.Double(aValue.x);
		aBase.Key("y");
		aBase.Double(aValue.y);
		aBase.Key("z");
		aBase.Double(aValue.z);
		aBase.Key("w");
		aBase.Double(aValue.w);
		aBase.EndObject();
	}

	inline void Serialize(const char* aVariableName, const CommonUtilities::Matrix4x4<float>& aValue, rapidjson::Writer<rapidjson::StringBuffer>& aBase)
	{
		aBase.Key(aVariableName);
		aBase.StartArray();
		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 4; j++)
				aBase.Double(aValue(i + 1, j + 1));
		aBase.EndArray();
	}
}

namespace Deserialize
{
	inline bool Bool(const rapidjson::Value::Object& aContainer, const char* aBoolName, bool aDefault = false)
	{
		if (aContainer.HasMember(aBoolName))
		{
			if (aContainer[aBoolName].IsBool())
			{
				return aContainer[aBoolName].GetBool();
			}
			Debug::Warning << aBoolName << " failed to deserialize, value isn't bool" << std::endl;
		}
		else
		{
			Debug::Warning << aBoolName << " failed to deserialize, value doesn't exist. Check spelling." << std::endl;
		}
		return aDefault;
	}

	inline float Float(const rapidjson::Value::Object& aContainer, const char* aFloatName, float aDefault = 0)
	{
		if (aContainer.HasMember(aFloatName))
		{
			if (aContainer[aFloatName].IsFloat() || aContainer[aFloatName].IsInt())
			{
				return aContainer[aFloatName].GetFloat();
			}
			Debug::Warning << aFloatName << " failed to deserialize, value isn't float" << std::endl;
		}
		else
		{
			Debug::Warning << aFloatName << " failed to deserialize, value doesn't exist. Check spelling." << std::endl;
		}
		return aDefault;
	}

	inline int Int(const rapidjson::Value::Object& aContainer, const char* aIntName, int aDefault = 0)
	{
		if (aContainer.HasMember(aIntName))
		{
			if (aContainer[aIntName].IsInt())
			{
				return aContainer[aIntName].GetInt();
			}
			Debug::Warning << aIntName << " failed to deserialize, value isn't int" << std::endl;
		}
		else
		{
			Debug::Warning << aIntName << " failed to deserialize, value doesn't exist. Check spelling." << std::endl;
		}
		return aDefault;
	}

	inline std::string String(const rapidjson::Value::Object& aContainer, const char* aStringName, const char* aDefault = "")
	{
		if (aContainer.HasMember(aStringName))
		{
			if (aContainer[aStringName].IsString() || aContainer[aStringName].IsFloat() || aContainer[aStringName].IsInt())
			{
				return aContainer[aStringName].GetString();
			}
			Debug::Warning << aStringName << " failed to deserialize, value isn't string" << std::endl;
		}
		else
		{
			Debug::Warning << aStringName << " failed to deserialize, value doesn't exist. Check spelling." << std::endl;
		}
		return aDefault;
	}

	inline CommonUtilities::Vector2<float> Vector2(const rapidjson::Value::Object& aContainer, const char* aVectorName, const CommonUtilities::Vector2<float>& aDefault = { 0, 0 })
	{
		if (aContainer.HasMember(aVectorName))
		{
			if (aContainer[aVectorName].IsObject())
			{
				return {
					Float(aContainer[aVectorName].GetObject(), "x"),
					Float(aContainer[aVectorName].GetObject(), "y")
				};
			}
			Debug::Warning << aVectorName << " failed to deserialize, value isn't vector" << std::endl;
		}
		else
		{
			Debug::Warning << aVectorName << " failed to deserialize, value doesn't exist. Check spelling." << std::endl;
		}
		return aDefault;
	}

	inline CommonUtilities::Vector3<float> Vector3(const rapidjson::Value::Object& aContainer, const char* aVectorName, const CommonUtilities::Vector3<float>& aDefault = { 0, 0, 0 })
	{
		if (aContainer.HasMember(aVectorName))
		{
			if (aContainer[aVectorName].IsObject())
			{
				return {
					Float(aContainer[aVectorName].GetObject(), "x"),
					Float(aContainer[aVectorName].GetObject(), "y"),
					Float(aContainer[aVectorName].GetObject(), "z")
				};
			}
			Debug::Warning << aVectorName << " failed to deserialize, value isn't vector" << std::endl;
		}
		else
		{
			Debug::Warning << aVectorName << " failed to deserialize, value doesn't exist. Check spelling." << std::endl;
		}
		return aDefault;
	}
	
	inline CommonUtilities::Vector4<float> Vector4(const rapidjson::Value::Object& aContainer, const char* aVectorName, const CommonUtilities::Vector4<float>& aDefault = { 0, 0, 0, 0 })
	{
		if (aContainer.HasMember(aVectorName))
		{
			if (aContainer[aVectorName].IsObject())
			{
				return {
					Float(aContainer[aVectorName].GetObject(), "x"),
					Float(aContainer[aVectorName].GetObject(), "y"),
					Float(aContainer[aVectorName].GetObject(), "z"),
					Float(aContainer[aVectorName].GetObject(), "w")
				};
			}
			Debug::Warning << aVectorName << " failed to deserialize, value isn't vector" << std::endl;
		}
		else
		{
			Debug::Warning << aVectorName << " failed to deserialize, value doesn't exist. Check spelling." << std::endl;
		}
		return aDefault;
	}

	inline CommonUtilities::Matrix4x4<float> Matrix(const rapidjson::Value::Object& aContainer, const char* aVectorName)
	{
		CommonUtilities::Matrix4x4<float> matrix;
		if (aContainer.HasMember(aVectorName))
		{
			if (aContainer[aVectorName].IsArray())
			{
				int row = 0;
				int column = 0;
				for(auto& it : aContainer[aVectorName].GetArray())
				{
					matrix(row + 1, column + 1) = it.GetDouble();
					column++;
					if(column >= 4)
					{
						column = 0;
						row++;
					}
				}
				return matrix;
			}
			Debug::Warning << aVectorName << " failed to deserialize, value isn't matrix" << std::endl;
		}
		else
		{
			Debug::Warning << aVectorName << " failed to deserialize, value doesn't exist. Check spelling." << std::endl;
		}
		return matrix;
	}
}