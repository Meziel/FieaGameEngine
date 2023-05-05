#include "pch.h"
#include "AttributedBar.h"
#include <cstddef>

using namespace FieaGameEngine;
using namespace std::string_literals;

namespace UnitTests
{
    ATTRIBUTED_DEFINITIONS(AttributedBar, FieaGameEngine::Attributed);

    AttributedBar::AttributedBar() :
        Attributed(this)
    {
    }

    AttributedBar::AttributedBar(const AttributedBar& rhs) :
        ExternalInteger(rhs.ExternalInteger),
        Attributed(rhs, this)
    {
    }

    AttributedBar::AttributedBar(AttributedBar&& rhs) noexcept :
        ExternalInteger(rhs.ExternalInteger),
        Attributed(std::move(rhs), this)
    {
    }

    std::unique_ptr<Scope> AttributedBar::Clone() const
    {
        return std::make_unique<AttributedBar>(*this);
    }		

    Vector<Signature> AttributedBar::Signatures()
    {
        return
        {
            { "ExternalInteger", DatumTypes::Integer, true, Signature::ExternalInfo{1, offsetof(AttributedBar, ExternalInteger)} }
        };
    }
}