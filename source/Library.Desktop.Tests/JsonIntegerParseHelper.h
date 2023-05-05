#pragma once

#include "IJsonParseHelper.h"
#include "Vector.h"

namespace UnitTests
{
	class JsonIntegerParseHelper final : public FieaGameEngine::IJsonParseHelper 
	{
        RTTI_DECLARATIONS(JsonTestParseHelper, FieaGameEngine::IJsonParseHelper);

    public:
        class Wrapper final : public FieaGameEngine::JsonParseCoordinator::Wrapper
        {
            RTTI_DECLARATIONS(Wrapper, ::FieaGameEngine::JsonParseCoordinator::Wrapper);

        public:
            [[nodiscard]] std::unique_ptr<FieaGameEngine::JsonParseCoordinator::Wrapper> Create() const override;
            [[nodiscard]] bool Equals(const RTTI* rhs) const override;

            FieaGameEngine::Vector<int32_t> Data;
        };

        std::unique_ptr<IJsonParseHelper> Create() const override;
        bool StartHandler(FieaGameEngine::JsonParseCoordinator::Wrapper& sharedData, const std::string& key, const Json::Value& value, std::size_t index = 0) override;
        bool EndHandler(FieaGameEngine::JsonParseCoordinator::Wrapper& sharedData, const std::string& key) override;
	};
}