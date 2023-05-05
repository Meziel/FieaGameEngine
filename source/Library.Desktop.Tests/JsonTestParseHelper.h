#pragma once

#include "IJsonParseHelper.h"

namespace UnitTests
{
    class JsonTestParseHelper final : public FieaGameEngine::IJsonParseHelper
    {
        RTTI_DECLARATIONS(JsonTestParseHelper, FieaGameEngine::IJsonParseHelper);
    
    public:
        class Wrapper final : public FieaGameEngine::JsonParseCoordinator::Wrapper
        {
            RTTI_DECLARATIONS(Wrapper, FieaGameEngine::JsonParseCoordinator::Wrapper);

        public:
            [[nodiscard]] std::unique_ptr<FieaGameEngine::JsonParseCoordinator::Wrapper> Create() const override;
            [[nodiscard]] bool Equals(const RTTI* rhs) const override;

            std::size_t MaxDepth = 0;
        };

        [[nodiscard]] std::unique_ptr<IJsonParseHelper> Create() const override;
        void Initialize(FieaGameEngine::JsonParseCoordinator::Wrapper& sharedData) override;
        void Cleanup(FieaGameEngine::JsonParseCoordinator::Wrapper& sharedData) override;
        bool StartHandler(FieaGameEngine::JsonParseCoordinator::Wrapper& sharedData, const std::string& key, const Json::Value& value, std::size_t index = 0) override;
        bool EndHandler(FieaGameEngine::JsonParseCoordinator::Wrapper& sharedData, const std::string& key) override;
    
        bool InitializeCalled = false;
        bool CleanupCalled = false;
        std::size_t StartHandlerCount = 0;
        std::size_t EndHandlerCount = 0;
    };
}