#pragma once
#include "document.h"
#include "Math/MathTypes.h"
#include <string>
#include <vector>

#define JSON_READ(value, data) ds::json::Get(value, #data, data);

namespace ds
{
	namespace json
	{
		bool Load(const std::string& filename, rapidjson::Document& document);
		bool Get(const rapidjson::Value& value, const std::string& name, int& data);
		bool Get(const rapidjson::Value& value, const std::string& name, float& data);
		bool Get(const rapidjson::Value& value, const std::string& name, bool& data);
		bool Get(const rapidjson::Value& value, const std::string& name, std::string& data);
		bool Get(const rapidjson::Value& value, const std::string& name, glm::vec2& data);
		bool Get(const rapidjson::Value& value, const std::string& name, glm::vec3& data);
		bool Get(const rapidjson::Value& value, const std::string& name, glm::vec4& data);
		bool Get(const rapidjson::Value& value, const std::string& name, std::vector<std::string>& data);
		bool Get(const rapidjson::Value& value, const std::string& name, std::vector<int>& data);
	}
}