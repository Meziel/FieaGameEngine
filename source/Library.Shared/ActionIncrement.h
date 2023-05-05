#pragma once

#include "ActionList.h"

namespace FieaGameEngine
{
    class ActionIncrement final : public Action
    {
        ATTRIBUTED_DECLARATIONS(ActionIncrement, FieaGameEngine::Action);

    public:
        ActionIncrement();
        ActionIncrement(const ActionIncrement& rhs);
        ActionIncrement(ActionIncrement&& rhs) noexcept;
        ActionIncrement& operator=(const ActionIncrement& rhs) = default;
        ActionIncrement& operator=(ActionIncrement&& rhs) noexcept = default;

        /// <summary>
        /// Attribute to increment
        /// </summary>
        std::string Target;

        /// <summary>
        /// Polymorphicly creates a copy of the ActionListIf
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

    private:
        template<typename T>
        void IncrementAs(Datum& targetDatum, const Datum& stepDatum);
    };

    ConcreteFactory(ActionIncrement, Scope);
}