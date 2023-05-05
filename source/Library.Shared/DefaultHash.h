#pragma once

#include <string>

namespace FieaGameEngine
{
	/// <summary>
	/// Default template for hash
	/// </summary>
	template <typename TKey>
	struct DefaultHash
	{
		size_t operator()(const TKey& key);
	};

	/// <summary>
	/// Char array specialization
	/// </summary>
	template <>
	struct DefaultHash<const char* const>
	{
		size_t operator()(const char* const key);
	};

	/// <summary>
	/// Char array specialization
	/// </summary>
	template <>
	struct DefaultHash<char* const>
	{
		size_t operator()(const char* const key);
	};

	/// <summary>
	/// Char array specialization
	/// </summary>
	template <>
	struct DefaultHash<const char*>
	{
		size_t operator()(const char* key);
	};

	/// <summary>
	/// Char array specialization
	/// </summary>
	template <>
	struct DefaultHash<char*>
	{
		size_t operator()(const char* const key);
	};

	/// <summary>
	/// String specialization
	/// </summary>
	template <>
	struct DefaultHash<const std::string>
	{
		size_t operator()(const std::string& key);
	};

	/// <summary>
	/// String specialization
	/// </summary>
	template <>
	struct DefaultHash<std::string>
	{
		size_t operator()(const std::string& key);
	};
}

#include "DefaultHash.inl"