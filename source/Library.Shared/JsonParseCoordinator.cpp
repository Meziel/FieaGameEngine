#include "pch.h"
#include "JsonParseCoordinator.h"
#include "IJsonParseHelper.h"
#include <fstream>

using namespace std;

namespace FieaGameEngine
{
    RTTI_DEFINITIONS(JsonParseCoordinator::Wrapper);

    void JsonParseCoordinator::Wrapper::Initialize()
    {
        _depth = 0;
    }

    void JsonParseCoordinator::Wrapper::Cleanup()
    {
        _depth = 0;
    }

    void JsonParseCoordinator::Wrapper::SetJsonParseCoordinator(JsonParseCoordinator* jsonParseCoordinator)
    {
        _jsonParseCoordinator = jsonParseCoordinator;
    }

    JsonParseCoordinator* JsonParseCoordinator::Wrapper::GetJsonParseCoordinator()
    {
        return _jsonParseCoordinator;
    }

    const JsonParseCoordinator* JsonParseCoordinator::Wrapper::GetJsonParseCoordinator() const
    {
        return _jsonParseCoordinator;
    }
    
    void JsonParseCoordinator::Wrapper::IncrementDepth()
    {
        ++_depth;
    }
    
    void JsonParseCoordinator::Wrapper::DecrementDepth()
    {
        --_depth;
    }
    
    std::size_t JsonParseCoordinator::Wrapper::Depth() const
    {
        return _depth;
    }

    JsonParseCoordinator::JsonParseCoordinator(std::shared_ptr<Wrapper> wrapper) :
        _wrapper(std::move(wrapper))
    {
        _wrapper->SetJsonParseCoordinator(this);
    }

    JsonParseCoordinator::JsonParseCoordinator(JsonParseCoordinator&& rhs) noexcept :
        _helpers(std::move(rhs._helpers)),
        _wrapper(std::move(rhs._wrapper))
    {
        _wrapper->SetJsonParseCoordinator(this);
    }

    JsonParseCoordinator& JsonParseCoordinator::operator=(JsonParseCoordinator&& rhs) noexcept
    {
        if (&rhs != this)
        {
            _helpers = std::move(rhs._helpers);
            _wrapper = std::move(rhs._wrapper);
            _wrapper->SetJsonParseCoordinator(this);
        }
        return *this;
    }

    std::unique_ptr<JsonParseCoordinator> JsonParseCoordinator::Clone() const
    {
        std::unique_ptr<JsonParseCoordinator> clone = make_unique<JsonParseCoordinator>(_wrapper->Create());
        clone->_helpers.Reserve(_helpers.Size());
        for (const std::shared_ptr<IJsonParseHelper>& helper : _helpers)
        {
            clone->AddHelper(helper->Create());
        }
        return clone;
    }

    void JsonParseCoordinator::AddHelper(std::shared_ptr<IJsonParseHelper> helper)
    {
        // TODO: Prevent same type ids
        _helpers.PushBack(std::move(helper));   
    }

    void JsonParseCoordinator::RemoveHelper(std::shared_ptr<IJsonParseHelper> helper)
    {
        // TODO: the parameter should be a RTTI:IdType instead of helper.
        // TODO: make second remove helpers
        _helpers.Remove(std::move(helper));
    }

    Vector<std::shared_ptr<IJsonParseHelper>>& JsonParseCoordinator::Helpers()
    {
        return _helpers;
    }

    const Vector<std::shared_ptr<IJsonParseHelper>>& JsonParseCoordinator::Helpers() const
    {
        return _helpers;
    }

    void JsonParseCoordinator::DeserializeObject(const std::string& json)
    {
        istringstream iss(json);
        DeserializeObject(iss);
    }

    void JsonParseCoordinator::DeserializeObjectFromFile(const std::string& file)
    {
        ifstream ifs(file);
        if (!ifs.good())
        {
            throw runtime_error("Failed to open json file");
        }
        DeserializeObject(ifs);
    }

    void JsonParseCoordinator::DeserializeObject(std::istream& stream)
    {
        _wrapper->Initialize();
        assert(_wrapper->Depth() == 0);

        // Initialize helpers
        for (std::shared_ptr<IJsonParseHelper>& helper : _helpers)
        {
            helper->Initialize(*_wrapper);
        }
        Json::Value root;
        stream >> root;
        ParseMembers(root);

        // Cleanup helpers
        for (std::shared_ptr<IJsonParseHelper>& helper : _helpers)
        {
            helper->Cleanup(*_wrapper);
        }

        assert(_wrapper->Depth() == 0);
        _wrapper->Cleanup();
    }

    JsonParseCoordinator::Wrapper& JsonParseCoordinator::GetWrapper()
    {
        return *_wrapper;
    }

    void JsonParseCoordinator::SetWrapper(std::shared_ptr<Wrapper> wrapper)
    {
        _wrapper = std::move(wrapper);
        _wrapper->SetJsonParseCoordinator(this);
    }

    void JsonParseCoordinator::ParseMembers(const Json::Value& value, std::size_t arrayIndex)
    {
        for (auto it = value.begin(); it != value.end(); ++it)
        {
            ParseKeyValuePair(it.key().asString(), *it, arrayIndex);
        }
    }

    void JsonParseCoordinator::ParseArray(const std::string& key, const Json::Value& value, std::size_t /*arrayIndex*/)
    {
        std::size_t index = 0;
        for (const Json::Value& arrayElement : value)
        {
            if (arrayElement.isArray())
            {
                ParseArray(key, arrayElement, index);
            }
            else if(arrayElement.isObject())
            {
                _wrapper->IncrementDepth();

                // Send key value to parse helpers
                for (std::shared_ptr<IJsonParseHelper>& helper : _helpers)
                {
                    bool canHandle = helper->StartHandler(*_wrapper.get(), key, value, index);
                    if (canHandle)
                    {
                        ParseMembers(arrayElement, index);
                        helper->EndHandler(*_wrapper.get(), key);
                        break;
                    }
                }

                _wrapper->DecrementDepth();
            }
            ++index;
        }
    }

    void JsonParseCoordinator::ParseKeyValuePair(const std::string& key, const Json::Value& value, std::size_t arrayIndex)
    {
        // TODO: Create helper function to remove duplicate code
        if (value.isObject())
        {
            _wrapper->IncrementDepth();
            for (std::shared_ptr<IJsonParseHelper>& helper : _helpers)
            {
                bool canHandle = helper->StartHandler(*_wrapper.get(), key, value, arrayIndex);
                if (canHandle)
                {
                    ParseMembers(value, arrayIndex);
                    helper->EndHandler(*_wrapper.get(), key);
                    break;
                }
            }
            _wrapper->DecrementDepth();
        }
        else if (value.isArray())
        {
            for (std::shared_ptr<IJsonParseHelper>& helper : _helpers)
            {
                bool canHandle = helper->StartHandler(*_wrapper.get(), key, value, arrayIndex);
                if (canHandle)
                {
                    ParseArray(key, value);
                    helper->EndHandler(*_wrapper.get(), key);
                    break;
                }
            }
        }
        else
        {
            for (std::shared_ptr<IJsonParseHelper>& helper : _helpers)
            {
                bool canHandle = helper->StartHandler(*_wrapper.get(), key, value, arrayIndex);
                if (canHandle)
                {
                    helper->EndHandler(*_wrapper.get(), key);
                    break;
                }
            }
        }
    }
}