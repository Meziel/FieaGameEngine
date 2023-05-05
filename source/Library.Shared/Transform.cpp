#include "pch.h"

#include "Transform.h"

using namespace std;
using namespace std::string_literals;

namespace FieaGameEngine
{
    ATTRIBUTED_DEFINITIONS(Transform, FieaGameEngine::Attributed);

    Transform::Transform() : Attributed(this)
    {
    }

    Transform::Transform(const Transform& rhs) :
        Position(rhs.Position),
        Rotation(rhs.Rotation),
        Scale(rhs.Scale),
        Attributed(rhs, this)
    {
    }

    Transform::Transform(Transform&& rhs) noexcept :
        Position(move(rhs.Position)),
        Rotation(move(rhs.Rotation)),
        Scale(move(rhs.Scale)),
        Attributed(move(rhs), this)
    {
    }

    std::unique_ptr<Scope> Transform::Clone() const
    {
        return make_unique<Transform>(*this);
    }

    Vector<Signature> Transform::Signatures()
    {
        return
        {
            {"Position", DatumTypes::Vector, true, Signature::ExternalInfo{1, offsetof(Transform, Position)}},
            {"Rotation", DatumTypes::Vector, true, Signature::ExternalInfo{1, offsetof(Transform, Rotation)}},
            {"Scale", DatumTypes::Vector, true, Signature::ExternalInfo{1, offsetof(Transform, Scale)}}
        };
    }
}