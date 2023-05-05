#include "pch.h"
#include "IJsonParseHelper.h"

namespace FieaGameEngine
{
    RTTI_DEFINITIONS(IJsonParseHelper);

    void IJsonParseHelper::Initialize(JsonParseCoordinator::Wrapper& /*sharedData*/)
    {
    }

    void IJsonParseHelper::Cleanup(JsonParseCoordinator::Wrapper& /*sharedData*/)
    {
    }

    bool IJsonParseHelper::Equals(const RTTI* /*rhs*/) const
    {
        return false;
    }
}