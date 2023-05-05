#include "pch.h"
#include "JsonIntegerParseHelper.h"

using namespace FieaGameEngine;
using namespace std;

namespace UnitTests
{
    RTTI_DEFINITIONS(JsonIntegerParseHelper);
    RTTI_DEFINITIONS(JsonIntegerParseHelper::Wrapper);

    std::unique_ptr<FieaGameEngine::JsonParseCoordinator::Wrapper> JsonIntegerParseHelper::Wrapper::Create() const
    {
        return make_unique<Wrapper>();
    }

    bool JsonIntegerParseHelper::Wrapper::Equals(const RTTI* rhs) const
    {  
        auto rhsAsWrapper = rhs->As<JsonIntegerParseHelper::Wrapper>();
        if (rhsAsWrapper == nullptr)
        {
            return false;
        }
        if (Data.Size() != rhsAsWrapper->Data.Size())
        {
            return false;
        }
        return std::equal(Data.begin(), Data.end(), rhsAsWrapper->Data.begin());
    }

    std::unique_ptr<IJsonParseHelper> JsonIntegerParseHelper::Create() const
    {
        return make_unique<JsonIntegerParseHelper>();
    }

    bool JsonIntegerParseHelper::StartHandler(FieaGameEngine::JsonParseCoordinator::Wrapper& sharedData, const std::string& /*key*/, const Json::Value& value, std::size_t /*index*/)
    {
        Wrapper* wrapper = sharedData.As<Wrapper>();
        if (wrapper == nullptr)
        {
            return false;
        }
        
        if (value.isInt())
        {
            wrapper->Data.PushBack(value.asInt());
        }
        else if(value.isArray())
        {
            // Check every element of array is a number
            for (const auto& arrayElement : value)
            {
                if (!arrayElement.isIntegral())
                {
                    return false;
                }
            }

            for (const auto& arrayElement : value)
            {
                wrapper->Data.PushBack(arrayElement.asInt());
            }
        }
        else
        {
            return false;
        }
        
        return true;
    }

    bool JsonIntegerParseHelper::EndHandler(FieaGameEngine::JsonParseCoordinator::Wrapper& /*sharedData*/, const std::string& /*key*/)
    {
        return true;
    }
};
