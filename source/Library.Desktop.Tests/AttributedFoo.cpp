#include "pch.h"
#include "AttributedFoo.h"
#include <cstddef>

using namespace FieaGameEngine;
using namespace std::string_literals;

namespace UnitTests
{
    ATTRIBUTED_DEFINITIONS(AttributedFoo, FieaGameEngine::Attributed);

    AttributedFoo::AttributedFoo() :
        Attributed(this)
    {
    }

    AttributedFoo::AttributedFoo(const AttributedFoo& rhs) :
        ExternalInteger(rhs.ExternalInteger),
        ExternalFloat(rhs.ExternalFloat),
        ExternalString(rhs.ExternalString),
        ExternalVector(rhs.ExternalVector),
        ExternalMatrix(rhs.ExternalMatrix),
        ExternalScope(rhs.ExternalScope),
        ExternalIntegerArray(rhs.ExternalIntegerArray),
        ExternalFloatArray(rhs.ExternalFloatArray),
        ExternalStringArray(rhs.ExternalStringArray),
        ExternalVectorArray(rhs.ExternalVectorArray),
        ExternalMatrixArray(rhs.ExternalMatrixArray),
        Attributed(rhs, this)
    {
    }

    AttributedFoo::AttributedFoo(AttributedFoo&& rhs) noexcept :
        ExternalInteger(rhs.ExternalInteger),
        ExternalFloat(rhs.ExternalFloat),
        ExternalString(std::move(rhs.ExternalString)),
        ExternalVector(std::move(rhs.ExternalVector)),
        ExternalMatrix(std::move(rhs.ExternalMatrix)),
        ExternalScope(std::move(rhs.ExternalScope)),
        ExternalIntegerArray(std::move(rhs.ExternalIntegerArray)),
        ExternalFloatArray(std::move(rhs.ExternalFloatArray)),
        ExternalStringArray(std::move(rhs.ExternalStringArray)),
        ExternalVectorArray(std::move(rhs.ExternalVectorArray)),
        ExternalMatrixArray(std::move(rhs.ExternalMatrixArray)),
        Attributed(std::move(rhs), this)
    {
    }

    std::unique_ptr<Scope> AttributedFoo::Clone() const
    {
        return std::make_unique<AttributedFoo>();
    }

    Vector<Signature> AttributedFoo::Signatures()
    {
        return Vector<Signature>(
        {
            { "ExternalInteger", DatumTypes::Integer, true, Signature::ExternalInfo{1, offsetof(AttributedFoo, ExternalInteger)}},
            { "ExternalFloat", DatumTypes::Float, true, Signature::ExternalInfo{1, offsetof(AttributedFoo, ExternalFloat)}},
            { "ExternalString", DatumTypes::String, true, Signature::ExternalInfo{1, offsetof(AttributedFoo, ExternalString)}},
            { "ExternalVector", DatumTypes::Vector, true, Signature::ExternalInfo{1, offsetof(AttributedFoo, ExternalVector)}},
            { "ExternalMatrix", DatumTypes::Matrix, true, Signature::ExternalInfo{1, offsetof(AttributedFoo, ExternalMatrix)}},
            { "ExternalScope", DatumTypes::TablePointer, true, Signature::ExternalInfo{1, offsetof(AttributedFoo, ExternalScope)}},

            { "ExternalIntegerArray", DatumTypes::Integer, true, Signature::ExternalInfo{2, offsetof(AttributedFoo, ExternalIntegerArray) }},
            { "ExternalFloatArray", DatumTypes::Float, true, Signature::ExternalInfo{2, offsetof(AttributedFoo, ExternalFloatArray) }},
            { "ExternalStringArray", DatumTypes::String, true, Signature::ExternalInfo{2, offsetof(AttributedFoo, ExternalStringArray) }},
            { "ExternalVectorArray", DatumTypes::Vector, true, Signature::ExternalInfo{2, offsetof(AttributedFoo, ExternalVectorArray) }},
            { "ExternalMatrixArray", DatumTypes::Matrix, true, Signature::ExternalInfo{2, offsetof(AttributedFoo, ExternalMatrixArray) }},
        });
    }
}