#pragma once

#include <vector>
#include <cstdint>
#include "Attributed.h"
#include "Factory.h"

namespace FieaGameEngine
{
    class Transform : public Attributed
    {
        ATTRIBUTED_DECLARATIONS(Transform, FieaGameEngine::Attributed);

    public:

        Transform();
        Transform(const Transform& rhs);
        Transform(Transform&& rhs) noexcept;
        Transform& operator=(const Transform& rhs) = default;
        Transform& operator=(Transform&& rhs) noexcept = default;

        glm::vec4 Position = glm::vec4(0);
        glm::vec4 Rotation = glm::vec4(0);
        glm::vec4 Scale = glm::vec4(0);

        /// <summary>
        /// Polymorphicly creates a copy of the Transform
        /// </summary>
        /// <returns></returns>
        [[nodiscard]] std::unique_ptr<Scope> Clone() const override;

        static Vector<Signature> Signatures();
    };

    ConcreteFactory(Transform, FieaGameEngine::Scope);
}