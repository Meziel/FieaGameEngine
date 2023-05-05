#pragma once

#include "Attributed.h"
#include "GameTime.h"

namespace FieaGameEngine
{
    class Action : public Attributed
    {
        ATTRIBUTED_DECLARATIONS(Action, FieaGameEngine::Attributed);

    public:
        /// <summary>
        /// Name of the Action
        /// </summary>
        std::string Name;

        /// <summary>
        /// Lists of prescribed attributes for this class
        /// </summary>
        /// <returns></returns>
        [[nodiscard]] static Vector<Signature> Signatures();

        /// <summary>
        /// Game loop called every tick
        /// </summary>
        /// <param name="time"></param>
        virtual void Update(const GameTime& time) = 0;
    protected:

        /// <summary>
        /// Constructor.
        /// When inheriting from this class, dervived constructors must call this method
        /// and pass in 'this' into the derivedThis parameter. This parameter allows this class
        /// to called derived class static methods inside the constructor.
        /// </summary>
        /// <param name="derivedThis">Pass in 'this' as an argument to this param</param>
        template<std::derived_from<Attributed> T>
        explicit Action(T* derivedThis);

        /// <summary>
        /// Copy constructor.
        /// When inheriting from this class, dervived copy constructors must call this method
        /// and pass in 'this' into the derivedThis parameter. This parameter allows this class
        /// to called derived class static methods inside the constructor.
        /// </summary>
        /// <param name="rhs">Right hand side to copy</param>
        /// <param name="derivedThis">Pass in 'this' as an argument to this param</param>
        template<std::derived_from<Attributed> T>
        Action(const Action& rhs, T* derivedThis);

        /// <summary>
        /// Move constructor.
        /// When inheriting from this class, dervived copy constructors must call this method
        /// and pass in 'this' into the derivedThis parameter. This parameter allows this class
        /// to called derived class static methods inside the constructor.
        /// </summary>
        /// <param name="rhs">Right hand side to move</param>
        /// <param name="derivedThis">Pass in 'this' as an argument to this param</param>
        template<std::derived_from<Attributed> T>
        Action(Action&& rhs, T* derivedThis) noexcept;

        Action& operator=(const Action& rhs) = default;
        Action& operator=(Action&& rhs) noexcept = default;
    };
}

#include "Action.inl"