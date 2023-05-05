#include "pch.h"
#include "format"
#include "JsonTableParseHelper.h"
#include "JsonValueSpecializations.h"
#include <regex>

using namespace std;

namespace FieaGameEngine
{
    RTTI_DEFINITIONS(JsonTableParseHelper);
    RTTI_DEFINITIONS(JsonTableParseHelper::Wrapper);

    std::unique_ptr<JsonParseCoordinator::Wrapper> JsonTableParseHelper::Wrapper::Create() const
    {
        return make_unique<Wrapper>();
    }

    bool JsonTableParseHelper::Wrapper::Equals(const RTTI* rhs) const
    {
        auto rhsAsWrapper = rhs->As<JsonTableParseHelper::Wrapper>();
        if (rhsAsWrapper == nullptr)
        {
            return false;
        }
        return Data == rhsAsWrapper->Data;
    }

    std::unique_ptr<IJsonParseHelper> JsonTableParseHelper::Create() const
    {
        return make_unique<JsonTableParseHelper>();
    }

    void JsonTableParseHelper::Initialize(JsonParseCoordinator::Wrapper& sharedData)
    {
        IJsonParseHelper::Initialize(sharedData);
        Wrapper* wrapper = sharedData.As<Wrapper>();
        if (wrapper != nullptr)
        {
            _tempWrapperScope = std::unique_ptr<Scope>(wrapper->Data->Clone());
        }
        _contextStack = {};
    }

    void JsonTableParseHelper::Cleanup(JsonParseCoordinator::Wrapper& sharedData)
    {
        IJsonParseHelper::Cleanup(sharedData);
        Wrapper* wrapper = sharedData.As<Wrapper>();
        if (wrapper != nullptr)
        {
            wrapper->Data = std::move(_tempWrapperScope);
        }
        _contextStack = {};
    }

    bool JsonTableParseHelper::StartHandler(JsonParseCoordinator::Wrapper& sharedData, const std::string& key, const Json::Value& value, std::size_t index)
    {
        Wrapper* wrapper = sharedData.As<Wrapper>();
        if (wrapper == nullptr)
        {
            return false;
        }

        if (wrapper->Data == nullptr)
        {
            return false;
        }

        if (value.isArray())
        {
            return true;
        }

        if (!value.isObject())
        {
            return false;
        }

        if (key == "value")
        {
            return true;
        }

        // If at root get scope from wrapper; otherwise get from stack
        Scope* parentScope;
        if (_contextStack.empty())
        {
            parentScope = _tempWrapperScope.get();
        }
        else
        {
            Datum& datumInContext = *&_contextStack.top().Datum;
            if (datumInContext.Type() == DatumTypes::TablePointer)
            {
                // External scopes
                parentScope = datumInContext.Get<Scope*>(index);
            }
            else
            {
                // Intenal scopes
                parentScope = datumInContext.Get<shared_ptr<Scope>>(index).get();
            }
        }

        const string TYPE_KEYWORD = "type";
        const string VALUE_KEYWORD = "value";

        // Get type
        const Json::Value* typeValue = value.find(TYPE_KEYWORD.c_str(), TYPE_KEYWORD.c_str() + TYPE_KEYWORD.size());
        if (typeValue == nullptr)
        {
            throw runtime_error(std::format("Could not find key 'type' in {}", key));
        }

        // Check type is a string
        if (!typeValue->isString())
        {
            throw runtime_error("Type was not a string");
        }

        const std::string& typeAsFullString = typeValue->asString();
        // Convert string to datum type
        std::smatch match;
        std::regex typeRegex(R"(^(\w+)(\[\])?$)");
        bool didTypeMatch = std::regex_match(typeAsFullString, match, typeRegex);
        if (!didTypeMatch)
        {
            throw runtime_error(std::format("{} is not a valid type", typeAsFullString));
        }
        const std::string& typeIdentifierAsString = match[1].str();
        bool isTypeArray = match[2].matched;
        auto stringToTypeIt = StringToDatumType.Find(typeIdentifierAsString);

        // DatumType does not exists
        if (stringToTypeIt == StringToDatumType.end())
        {
            throw runtime_error(std::format("Type {} is not a valid DatumType", typeIdentifierAsString));
        }

        const DatumTypes& type = stringToTypeIt->second;

        // Don't support unkown or pointer types
        if (type == DatumTypes::Unknown || type == DatumTypes::Pointer)
        {
            throw runtime_error(std::format("Type {} is not supported", typeIdentifierAsString));
        }

        // Get value
        const Json::Value* valueValue = value.find(VALUE_KEYWORD.c_str(), VALUE_KEYWORD.c_str() + VALUE_KEYWORD.size());
        if (valueValue == nullptr)
        {
            throw runtime_error(std::format("Could not find key 'value' in {}", key));
        }

        // Push onto context stack info about this datum
        Datum& datum = parentScope->Append(key);
        datum.SetType(type);
        _contextStack.push({ datum, key });

        switch (type)
        {
            case DatumTypes::Table:
                {
                    // Verify if the array is made up of objects
                    if (isTypeArray)
                    {
                        for (const auto& arrayElement : *valueValue)
                        {
                            if (!arrayElement.isObject())
                            {
                                throw runtime_error(std::format("Table array in {} has a non-table element", key));
                            }
                            if (!datum.IsExternal())
                            {
                                std::string classValueAsString = _GetClassNameFromTable(arrayElement);
                                parentScope->AppendScope(key, classValueAsString);
                            }
                        }
                    }
                    else
                    {
                        if (!datum.IsExternal())
                        {
                            std::string classValueAsString = _GetClassNameFromTable(value);
                            parentScope->AppendScope(key, classValueAsString);
                        }
                    }
                }
                break;
            case DatumTypes::TablePointer:
                // Don't do anything for external scopes
                break;
            default:
                _PopulateDatumWithPrimitive(datum, key, *valueValue, isTypeArray);
                break;
        }

        return true;
    }
    
    bool JsonTableParseHelper::EndHandler(JsonParseCoordinator::Wrapper& /*sharedData*/, const std::string& key)
    {
        if (!_contextStack.empty() && &_contextStack.top().DatumName == &key)
        {
            _contextStack.pop();
        }

        return true;
    }
    
    std::string JsonTableParseHelper::_GetClassNameFromTable(const Json::Value& value)
    {
        // Get class
        const string CLASS_KEYWORD = "class";
        string classValueAsString = "Scope";
        const Json::Value* classValue = value.find(CLASS_KEYWORD.c_str(), CLASS_KEYWORD.c_str() + CLASS_KEYWORD.size());
        if (classValue != nullptr)
        {
            if (!classValue->isString())
            {
                throw runtime_error("Field 'class' should be a string");
            }
            classValueAsString = classValue->asString();
        }
        return classValueAsString;
    }

    void JsonTableParseHelper::_PopulateDatumWithPrimitive(Datum& datum, const std::string& key, const Json::Value& value, bool isArray)
    {
        std::size_t numElement = 1;
        if (isArray)
        {
            numElement = value.size();
        }

        if (datum.IsExternal())
        {
            if (numElement != datum.Size())
            {
                throw runtime_error(std::format("{}'s json value element size does not match external datum's size", key));
            }
            if (!isArray)
            {
                _SetDatumFromJsonValue(datum, value);
            }
            else
            {
                for (uint32_t i = 0; i < numElement; ++i)
                {
                    _SetDatumFromJsonValue(datum, value[i], i);
                }
            }
        }
        else
        {
            // Complete overwrite if non-external
            datum.Clear();
            if (!isArray)
            {
                _PushBackDatumFromJsonValue(datum, value);
            }
            else
            {
                for (uint32_t i = 0; i < numElement; ++i)
                {
                    _PushBackDatumFromJsonValue(datum, value[i]);
                }
            }
        }
    }

    void JsonTableParseHelper::_SetDatumFromJsonValue(Datum& datum, const Json::Value& value, std::size_t index)
    {
        switch (datum.Type())
        {
            case DatumTypes::Boolean:
                datum.Set(FromJsonValue<bool>(value), index);
                break;
            case DatumTypes::Integer:
                datum.Set(FromJsonValue<int32_t>(value), index);
                break;
            case DatumTypes::Float:
                datum.Set(FromJsonValue<float>(value), index);
                break;
            case DatumTypes::String:
                datum.Set(FromJsonValue<std::string>(value), index);
                break;
            case DatumTypes::Vector:
                datum.Set(FromJsonValue<glm::vec4>(value), index);
                break;
            case DatumTypes::Matrix:
                datum.Set(FromJsonValue<glm::mat4>(value), index);
                break;
            default:
                break;
        }
    }
    
    void JsonTableParseHelper::_PushBackDatumFromJsonValue(Datum& datum, const Json::Value& value)
    {
        switch (datum.Type())
        {
            case DatumTypes::Boolean:
                datum.PushBack(FromJsonValue<bool>(value));
                break;
            case DatumTypes::Integer:
                datum.PushBack(FromJsonValue<int32_t>(value));
                break;
            case DatumTypes::Float:
                datum.PushBack(FromJsonValue<float>(value));
                break;
            case DatumTypes::String:
                datum.PushBack(FromJsonValue<std::string>(value));
                break;
            case DatumTypes::Vector:
                datum.PushBack(FromJsonValue<glm::vec4>(value));
                break;
            case DatumTypes::Matrix:
                datum.PushBack(FromJsonValue<glm::mat4>(value));
                break;
            default:
                break;
        }
    }
}