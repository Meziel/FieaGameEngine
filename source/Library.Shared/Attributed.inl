#include "Attributed.h"

namespace FieaGameEngine
{
	template<std::derived_from<Attributed> T>
	Attributed::Attributed(T*) : Scope()
	{
		Populate(T::AllSignatures());
	}

	template<std::derived_from<Attributed> T>
	Attributed::Attributed(const Attributed& rhs, T*) : Scope(rhs)
	{
		Populate(T::AllSignatures());
	}

	template<std::derived_from<Attributed> T>
	Attributed::Attributed(Attributed&& rhs, T*) noexcept : Scope(std::move(rhs))
	{
		Populate(T::AllSignatures());
	}
}