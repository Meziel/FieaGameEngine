#include "Scope.h"

namespace FieaGameEngine
{
	// Equality
	inline bool DefaultEquality<Scope*>::operator()(const Scope* lhs, const Scope* rhs) const
	{
		if (lhs == nullptr)
		{
			return rhs == nullptr;
		}
		return lhs->Equals(rhs);
	}

	inline bool DefaultEquality<const Scope*>::operator()(const Scope* lhs, const Scope* rhs) const
	{
		if (lhs == nullptr)
		{
			return rhs == nullptr;
		}
		return lhs->Equals(rhs);
	}

	inline bool DefaultEquality<Scope* const>::operator()(const Scope* lhs, const Scope* rhs) const
	{
		if (lhs == nullptr)
		{
			return rhs == nullptr;
		}
		return lhs->Equals(rhs);
	}

	inline bool DefaultEquality<const Scope* const>::operator()(const Scope* lhs, const Scope* rhs) const
	{
		if (lhs == nullptr)
		{
			return rhs == nullptr;
		}
		return lhs->Equals(rhs);
	}

	inline bool DefaultEquality<std::shared_ptr<Scope>>::operator()(const std::shared_ptr<Scope>& lhs, const std::shared_ptr<Scope>& rhs) const
	{
		if (lhs == nullptr)
		{
			return rhs == nullptr;
		}
		return lhs->Equals(rhs.get());
	}
}