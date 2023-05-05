#include "Action.h"

namespace FieaGameEngine
{
	template<std::derived_from<Attributed> T>
	Action::Action(T* t) : Attributed(t)
	{
	}

	template<std::derived_from<Attributed> T>
	Action::Action(const Action& rhs, T* t) :
		Name(rhs.Name),
		Attributed(rhs, t)
	{
	}

	template<std::derived_from<Attributed> T>
	Action::Action(Action&& rhs, T* t) noexcept :
		Name(std::move(rhs.Name)),
		Attributed(std::move(rhs), t)
	{
	}
}
