#pragma once

#include <cstddef>
#include <string>
#include <gsl/gsl>
#include "DefaultEquality.h"

namespace FieaGameEngine
{
	class RTTI
	{
	public:
		using IdType = std::size_t;

		RTTI() = default;
		RTTI(const RTTI&) = default;
		RTTI& operator=(const RTTI&) = default;
		RTTI(RTTI&&) noexcept = default;
		RTTI& operator=(RTTI&&) noexcept = default;
		virtual ~RTTI() = default;

		virtual IdType TypeIdInstance() const = 0;
		virtual bool Is(IdType) const;

		template <typename T>
		T* As();

		template <typename T>
		const T* As() const;

		bool operator==(const RTTI& rhs) const;
		bool operator!=(const RTTI& rhs) const;

		virtual std::string ToString() const;
		virtual bool Equals(const RTTI* rhs) const;
	};

	// Equality override for RTTI
	template <>
	struct DefaultEquality<RTTI*> final
	{
		bool operator()(const RTTI* lhs, const RTTI* rhs) const;
	};

	template <>
	struct DefaultEquality<const RTTI*> final
	{
		bool operator()(const RTTI* lhs, const RTTI* rhs) const;
	};

	template <>
	struct DefaultEquality<RTTI* const> final
	{
		bool operator()(const RTTI* lhs, const RTTI* rhs) const;
	};

	template <>
	struct DefaultEquality<const RTTI* const> final
	{
		bool operator()(const RTTI* lhs, const RTTI* rhs) const;
	};
}

#define RTTI_DECLARATIONS(Type, ParentType)																					\
	public:														                                                            \
		static FieaGameEngine::RTTI::IdType TypeIdClass() { return _typeId; }												\
		FieaGameEngine::RTTI::IdType TypeIdInstance() const override { return TypeIdClass(); }								\
		bool Is(FieaGameEngine::RTTI::IdType id) const override { return (id == _typeId ? true : ParentType::Is(id)); }		\
        static std::string TypeName() { return #Type; }	                                                                    \
	private:																												\
		static const FieaGameEngine::RTTI::IdType _typeId

#define RTTI_DEFINITIONS(Type)																								\
	const FieaGameEngine::RTTI::IdType Type::_typeId = reinterpret_cast<FieaGameEngine::RTTI::IdType>(&Type::_typeId)
	

#include "RTTI.inl"