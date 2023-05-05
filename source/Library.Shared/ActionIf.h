#pragma once

#include "ActionList.h"

namespace FieaGameEngine
{
    class ActionIf final : public Action
    {
        ATTRIBUTED_DECLARATIONS(ActionIf, FieaGameEngine::Action);

    public:
        ActionIf();
        ActionIf(const ActionIf& rhs);
        ActionIf(ActionIf&& rhs) noexcept;
        ActionIf& operator=(const ActionIf& rhs) = default;
        ActionIf& operator=(ActionIf&& rhs) noexcept = default;

        /// <summary>
        /// Indicates if the if statement is true or false
        /// </summary>
        bool Condition = false;

        /// <summary>
        /// Polymorphicly creates a copy of the ActionIf
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
    };

    ConcreteFactory(ActionIf, Scope);
}