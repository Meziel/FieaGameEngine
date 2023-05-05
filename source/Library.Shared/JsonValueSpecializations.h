#pragma once

#include <json/json.h>
#include <glm/glm.hpp>
#include <cstdint>
#include <string>
#include <stdexcept>
#include <Datum.h>

namespace FieaGameEngine
{
	/// <summary>
	/// Converts JsonValue to a type
	/// </summary>
	/// <typeparam name="T">Type to convert to</typeparam>
	/// <param name="jsonValue">Json value to convert from</param>
	/// <returns>Json value converted to specified type</returns>
	template <typename T>
	inline T FromJsonValue(Json::Value) {
		return T{};
	}

	template<>
	inline bool FromJsonValue<bool>(Json::Value jsonValue)
	{
		return jsonValue.asBool();
	}

	template<>
	inline std::int32_t FromJsonValue<std::int32_t>(Json::Value jsonValue)
	{
		return jsonValue.asInt();
	}

	template<>
	inline float FromJsonValue<float>(Json::Value jsonValue)
	{
		return jsonValue.asFloat();
	}

	template<>
	inline std::string FromJsonValue<std::string>(Json::Value jsonValue)
	{
		return jsonValue.asString();
	}

	template<>
	inline glm::vec4 FromJsonValue <glm::vec4> (Json::Value jsonValue)
	{
		glm::vec4 vec(0);
		if (!jsonValue.isArray())
		{
			throw std::runtime_error("");
		}
		if (jsonValue.size() != 4)
		{
			throw std::runtime_error("");
		}
		for (uint32_t i = 0; i < jsonValue.size(); ++i)
		{
			if (!jsonValue[i].isNumeric())
			{
				throw std::runtime_error("");
			}
			vec[i] = jsonValue[i].asFloat();
		}
		return vec;
	}

	template<>
	inline glm::mat4 FromJsonValue <glm::mat4>(Json::Value jsonValue)
	{
		glm::mat4 mat(0);
		if (!jsonValue.isArray())
		{
			throw std::runtime_error("");
		}
		if (jsonValue.size() != 4)
		{
			throw std::runtime_error("");
		}
		for (uint32_t row = 0; row < jsonValue.size(); ++row)
		{
			if (!jsonValue.isArray())
			{
				throw std::runtime_error("");
			}
			if (jsonValue.size() != 4)
			{
				throw std::runtime_error("");
			}
			for (uint32_t col = 0; col < jsonValue[row].size(); ++col)
			{
				if (!jsonValue[row][col].isNumeric())
				{
					throw std::runtime_error("");
				}
				mat[row][col] = jsonValue[row][col].asFloat();
			}
		}
		return mat;
	}
}
