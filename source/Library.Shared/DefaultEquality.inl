#include "DefaultEquality.h"
#include <string.h>
#include <functional>

namespace FieaGameEngine
{
	template<typename T>
	inline bool DefaultEquality<T>::operator()(const T& lhs, const T& rhs) const
	{
		return lhs == rhs;
	}

	inline bool DefaultEquality<char*>::operator()(const char* lhs, const char* rhs) const
	{
		return strcmp(lhs, rhs) == 0;
	}

	inline bool DefaultEquality<const char*>::operator()(const char* lhs, const char* rhs) const
	{
		return strcmp(lhs, rhs) == 0;
	}

	inline bool DefaultEquality<char* const>::operator()(const char* lhs, const char* rhs) const
	{
		return strcmp(lhs, rhs) == 0;
	}

	inline bool DefaultEquality<const char* const>::operator()(const char* lhs, const char* rhs) const
	{
		return strcmp(lhs, rhs) == 0;
	}
}