#pragma once

namespace FieaGameEngine
{
	/// <summary>
	/// Compares two generics
	/// </summary>
	/// <typeparam name="T"></typeparam>
	template <typename T>
	struct DefaultEquality final
	{
		bool operator()(const T& lhs, const T& rhs) const;
	};

	/// <summary>
	/// Compares two char pointers
	/// </summary>
	template <>
	struct DefaultEquality<char*> final
	{
		bool operator()(const char* lhs, const char* rhs) const;
	};

	/// <summary>
	/// Compares two char pointers
	/// </summary>
	template <>
	struct DefaultEquality<const char*> final
	{
		bool operator()(const char* lhs, const char* rhs) const;
	};

	/// <summary>
	/// Compares two char pointers
	/// </summary>
	template <>
	struct DefaultEquality<char* const> final
	{
		bool operator()(const char* lhs, const char* rhs) const;
	};

	/// <summary>
	/// Compares two char pointers
	/// </summary>
	template <>
	struct DefaultEquality<const char* const> final
	{
		bool operator()(const char* lhs, const char* rhs) const;
	};
}

#include "DefaultEquality.inl"