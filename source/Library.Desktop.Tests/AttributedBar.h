#pragma once

#include <cstdint>
#include <string>
#include <glm/glm.hpp>
#include "Attributed.h"
#include "Signature.h"
#include "Vector.h"

namespace UnitTests
{
    class AttributedBar final : public FieaGameEngine::Attributed
    {
        ATTRIBUTED_DECLARATIONS(AttributedBar, FieaGameEngine::Attributed);

    public:
        AttributedBar();
        AttributedBar(const AttributedBar& rhs);
        AttributedBar(AttributedBar&& rhs) noexcept;
        AttributedBar& operator=(const AttributedBar& rhs) = default;
        AttributedBar& operator=(AttributedBar&& rhs) noexcept = default;

        // Prescribed attributed
        std::int32_t ExternalInteger = 0;

        [[nodiscard]] std::unique_ptr<Scope> Clone() const override; 
        [[nodiscard]] static FieaGameEngine::Vector<FieaGameEngine::Signature> Signatures();
    };
}