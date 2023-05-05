#include "Reaction.h"

namespace FieaGameEngine
{
    template<std::derived_from<Attributed> T>
    inline Reaction::Reaction(T* derivedThis) : ActionList(derivedThis)
    {
    }

    template<std::derived_from<Attributed> T>
    inline Reaction::Reaction(const Reaction& rhs, T* derivedThis) :
        ActionList(rhs, derivedThis)
    {
    }

    template<std::derived_from<Attributed> T>
    inline Reaction::Reaction(Reaction&& rhs, T* derivedThis) noexcept :
        ActionList(std::move(rhs), derivedThis)
    {
    }
}