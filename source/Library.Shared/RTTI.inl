#include "RTTI.h"

namespace FieaGameEngine
{
	inline bool RTTI::Is(IdType) const
	{
		return false;
	}

	template <typename T>
	inline const T* RTTI::As() const
	{
		return (Is(T::TypeIdClass()) ? reinterpret_cast<const T*>(this) : nullptr);
	}

	template <typename T>
	inline T* RTTI::As()
	{
		return (Is(T::TypeIdClass()) ? reinterpret_cast<T*>(const_cast<RTTI*>(this)) : nullptr);
	}

	// Equality
	inline bool DefaultEquality<RTTI*>::operator()(const RTTI* lhs, const RTTI* rhs) const
	{
		if (lhs == nullptr)
		{
			return rhs == nullptr;
		}
		return lhs->Equals(rhs);
	}

	inline bool DefaultEquality<const RTTI*>::operator()(const RTTI* lhs, const RTTI* rhs) const
	{
		if (lhs == nullptr)
		{
			return rhs == nullptr;
		}
		return lhs->Equals(rhs);
	}

	inline bool DefaultEquality<RTTI* const>::operator()(const RTTI* lhs, const RTTI* rhs) const
	{
		if (lhs == nullptr)
		{
			return rhs == nullptr;
		}
		return lhs->Equals(rhs);
	}

	inline bool DefaultEquality<const RTTI* const>::operator()(const RTTI* lhs, const RTTI* rhs) const
	{
		if (lhs == nullptr)
		{
			return rhs == nullptr;
		}
		return lhs->Equals(rhs);
	}
}