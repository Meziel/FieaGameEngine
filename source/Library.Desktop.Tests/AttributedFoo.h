#pragma once

#include <cstdint>
#include <string>
#include <glm/glm.hpp>
#include "Attributed.h"
#include "Signature.h"
#include "Vector.h"
#include "AttributedBar.h"
#include "Factory.h"

namespace UnitTests
{
    class AttributedFoo final : public FieaGameEngine::Attributed
    {
    ATTRIBUTED_DECLARATIONS(AttributedFoo, FieaGameEngine::Attributed);
    
    public:
        AttributedFoo();
        AttributedFoo(const AttributedFoo& rhs);
        AttributedFoo(AttributedFoo&& rhs) noexcept;
        AttributedFoo& operator=(const AttributedFoo& rhs) = default;
        AttributedFoo& operator=(AttributedFoo&& rhs) noexcept = default;

        // Prescribed attributed
        std::int32_t ExternalInteger = 0;
        float ExternalFloat{0};
        std::string ExternalString;
        glm::vec4 ExternalVector;
        glm::mat4 ExternalMatrix;
        AttributedBar ExternalScope;

        std::array<std::int32_t, 2> ExternalIntegerArray {0};
        std::array<float, 2> ExternalFloatArray {0};
        std::array<std::string, 2> ExternalStringArray {""};
        std::array<glm::vec4, 2> ExternalVectorArray {glm::vec4(0)};
        std::array<glm::mat4, 2> ExternalMatrixArray {glm::mat4(0)};

        [[nodiscard]] std::unique_ptr<Scope> Clone() const override; 
        [[nodiscard]] static FieaGameEngine::Vector<FieaGameEngine::Signature> Signatures();
    };

    ConcreteFactory(AttributedFoo, FieaGameEngine::Scope);
}