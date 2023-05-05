#pragma once

#include "Action.h"

namespace FieaGameEngine
{
    class ActionList : public Action
    {
        ATTRIBUTED_DECLARATIONS(ActionList, FieaGameEngine::Action);

    public:
        ActionList();
        ActionList(const ActionList& rhs);
        ActionList(ActionList&& rhs) noexcept;
        ActionList& operator=(const ActionList& rhs) = default;
        ActionList& operator=(ActionList&& rhs) noexcept = default;

        /// <summary>
        /// Gets a list of actions in the action list
        /// </summary>
        /// <returns>Actions within the action list</returns>
        Datum& Actions();

        /// <summary>
        /// Gets a list of actions in the action list
        /// </summary>
        /// <returns>Actions within the action list</returns>
        const Datum& Actions() const;

        /// <summary>
        /// Creates an action from a string and adds it the Actions attribute.
        /// Action string requires a a factory of the action type
        /// </summary>
        /// <param name="actionName">Action class to instantiate</param>
        Action& CreateAction(std::string actionClass, std::string name = "");

        /// <summary>
        /// Polymorphicly creates a copy of the ActionList
        /// </summary>
        /// <returns></returns>
        [[nodiscard]] std::unique_ptr<Scope> Clone() const override;

        /// <summary>
        /// Lists of prescribed attributes for this class
        /// </summary>
        /// <returns></returns>
        [[nodiscard]] static Vector<Signature> Signatures();

        /// <summary>
        /// Game loop called every tick
        /// </summary>
        /// <param name="time"></param>
        void Update(const GameTime& time) override;

    protected:
        /// <summary>
        /// Constructor.
        /// When inheriting from this class, dervived constructors must call this method
        /// and pass in 'this' into the derivedThis parameter. This parameter allows this class
        /// to called derived class static methods inside the constructor.
        /// </summary>
        /// <param name="derivedThis">Pass in 'this' as an argument to this param</param>
        template<std::derived_from<Attributed> T>
        explicit ActionList(T* derivedThis);

        /// <summary>
        /// Copy constructor.
        /// When inheriting from this class, dervived copy constructors must call this method
        /// and pass in 'this' into the derivedThis parameter. This parameter allows this class
        /// to called derived class static methods inside the constructor.
        /// </summary>
        /// <param name="rhs">Right hand side to copy</param>
        /// <param name="derivedThis">Pass in 'this' as an argument to this param</param>
        template<std::derived_from<Attributed> T>
        ActionList(const ActionList& rhs, T* derivedThis);

        /// <summary>
        /// Move constructor.
        /// When inheriting from this class, dervived copy constructors must call this method
        /// and pass in 'this' into the derivedThis parameter. This parameter allows this class
        /// to called derived class static methods inside the constructor.
        /// </summary>
        /// <param name="rhs">Right hand side to move</param>
        /// <param name="derivedThis">Pass in 'this' as an argument to this param</param>
        template<std::derived_from<Attributed> T>
        ActionList(ActionList&& rhs, T* derivedThis) noexcept;
    };

    ConcreteFactory(ActionList, Scope);
}

#include "ActionList.inl"