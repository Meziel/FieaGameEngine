#pragma once

#include "ActionList.h"
#include "IEventSubscriber.h"

namespace FieaGameEngine
{
	class Reaction : public ActionList, public IEventSubscriber
	{
        ATTRIBUTED_DECLARATIONS(Reaction, ActionList);

    public:
        /// <summary>
        /// Lists of prescribed attributes for this class
        /// </summary>
        /// <returns></returns>
        [[nodiscard]] static Vector<Signature> Signatures();

    protected:
        /// <summary>
        /// Constructor.
        /// When inheriting from this class, dervived constructors must call this method
        /// and pass in 'this' into the derivedThis parameter. This parameter allows this class
        /// to called derived class static methods inside the constructor.
        /// </summary>
        /// <param name="derivedThis">Pass in 'this' as an argument to this param</param>
        template<std::derived_from<Attributed> T>
        explicit Reaction(T* derivedThis);

        /// <summary>
        /// Copy constructor.
        /// When inheriting from this class, dervived copy constructors must call this method
        /// and pass in 'this' into the derivedThis parameter. This parameter allows this class
        /// to called derived class static methods inside the constructor.
        /// </summary>
        /// <param name="rhs">Right hand side to copy</param>
        /// <param name="derivedThis">Pass in 'this' as an argument to this param</param>
        template<std::derived_from<Attributed> T>
        Reaction(const Reaction& rhs, T* derivedThis);

        /// <summary>
        /// Move constructor.
        /// When inheriting from this class, dervived copy constructors must call this method
        /// and pass in 'this' into the derivedThis parameter. This parameter allows this class
        /// to called derived class static methods inside the constructor.
        /// </summary>
        /// <param name="rhs">Right hand side to move</param>
        /// <param name="derivedThis">Pass in 'this' as an argument to this param</param>
        template<std::derived_from<Attributed> T>
        Reaction(Reaction&& rhs, T* derivedThis) noexcept;
	};
}

#include "Reaction.inl"