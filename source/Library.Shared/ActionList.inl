#include "ActionList.h"

namespace FieaGameEngine
{
	template<std::derived_from<Attributed> T>
	ActionList::ActionList(T* t) : Action(t)
	{
	}

	template<std::derived_from<Attributed> T>
	ActionList::ActionList(const ActionList& rhs, T* t) :
		Action(rhs, t)
	{
	}

	template<std::derived_from<Attributed> T>
	ActionList::ActionList(ActionList&& rhs, T* t) noexcept :
		Action(std::move(rhs), t)
	{
	}
}

