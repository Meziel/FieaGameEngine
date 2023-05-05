#include "pch.h"
#include "JsonTestParseHelper.h"

using namespace FieaGameEngine;
using namespace std;

namespace UnitTests
{
    RTTI_DEFINITIONS(JsonTestParseHelper);
    RTTI_DEFINITIONS(JsonTestParseHelper::Wrapper);

    std::unique_ptr<FieaGameEngine::JsonParseCoordinator::Wrapper> JsonTestParseHelper::Wrapper::Create() const
    {
        return make_unique<Wrapper>();
    }

    bool JsonTestParseHelper::Wrapper::Equals(const RTTI* /*rhs*/) const
    {
        return false;
    }

    std::unique_ptr<IJsonParseHelper> JsonTestParseHelper::Create() const
    {
        return make_unique<JsonTestParseHelper>();
    }

    void JsonTestParseHelper::Initialize(JsonParseCoordinator::Wrapper& sharedData)
    {
        IJsonParseHelper::Initialize(sharedData);
        InitializeCalled = true;
        CleanupCalled = false;
        StartHandlerCount = 0;
        EndHandlerCount = 0;
    }

    void JsonTestParseHelper::Cleanup(JsonParseCoordinator::Wrapper& sharedData)
    {
        IJsonParseHelper::Initialize(sharedData);
        CleanupCalled = true;
    }

    bool JsonTestParseHelper::StartHandler(FieaGameEngine::JsonParseCoordinator::Wrapper& sharedData, const std::string& /*key*/, const Json::Value& /*value*/, std::size_t /*index*/)
    {
        JsonTestParseHelper::Wrapper* wrapper = sharedData.As<JsonTestParseHelper::Wrapper>();
        if (wrapper == nullptr)
        {
            return false;
        }

        ++StartHandlerCount;

        if (wrapper->Depth() > wrapper->MaxDepth)
        {
            wrapper->MaxDepth = wrapper->Depth();
        }

        return true;
    }

    bool JsonTestParseHelper::EndHandler(FieaGameEngine::JsonParseCoordinator::Wrapper& /*sharedData*/, const std::string& /*key*/)
    {
        ++EndHandlerCount;
        return true;
    }
}