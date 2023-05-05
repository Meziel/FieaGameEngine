#pragma once

#include "IJsonParseHelper.h"
#include "Scope.h"
#include <stack>
#include <vector>

namespace FieaGameEngine
{
    /// <summary>
    /// Parse helper that can deserialize json into a scope.
    /// If the json contains datum names that are the same as the provided scope,
    /// then the datums will be clears and overwritten. If the datum names are marked external,
    /// then the size and type of the datum must match; otherwise an exception is thrown.
    /// If an exception is thrown, the original scope provided will not be modified.
    /// </summary>
    class JsonTableParseHelper final : public IJsonParseHelper
    {
        RTTI_DECLARATIONS(JsonTableParseHelper, IJsonParseHelper);

    public:
        /// <summary>
        /// Json parser wrapper that wraps a scope
        /// </summary>
        class Wrapper final : public JsonParseCoordinator::Wrapper
        {
            RTTI_DECLARATIONS(Wrapper, JsonParseCoordinator::Wrapper);

        public:
            /// <summary>
            /// Virtually constructs a wrapper of this type
            /// </summary>
            /// <returns>New wrapper</returns>
            [[nodiscard]] std::unique_ptr<JsonParseCoordinator::Wrapper> Create() const override;

            /// <summary>
            /// Compares two wrappers of this type
            /// </summary>
            /// <param name="rhs"></param>
            /// <returns>True if equal; otherwise false</returns>
            [[nodiscard]] bool Equals(const RTTI* rhs) const override;

            /// <summary>
            /// Wrapped scope
            /// </summary>
            std::shared_ptr<Scope> Data = nullptr;
        };

        /// <summary>
        /// Virtually constructs a IJsonParseHelper of this type
        /// </summary>
        /// <returns>New IJsonParseHelper</returns>
        [[nodiscard]] std::unique_ptr<IJsonParseHelper> Create() const override;

        /// <summary>
        /// Initializes the parse helper before it is used to deserialize
        /// </summary>
        /// <param name="sharedData">wrapper</param>
        void Initialize(JsonParseCoordinator::Wrapper& sharedData) override;

        /// <summary>
        /// Cleans up the parse helper after it has finished deserializing
        /// </summary>
        /// <param name="sharedData">wrapper</param>
        void Cleanup(JsonParseCoordinator::Wrapper& sharedData) override;

        /// <summary>
        /// Tries to handle a json key value pair and populate the provided Wrapper.
        /// If the helper can handle this kind of data, then this method should return true; otherwise false. 
        /// </summary>
        /// <param name="sharedData">Wrapper to handle</param>
        /// <param name="key">Key to handle</param>
        /// <param name="value">Value to handle</param>
        /// <param name="index">Array index of this element</param>
        /// <returns>True if the helper can parse this key value pair with this wrapper; otherwise false</returns>
        bool StartHandler(JsonParseCoordinator::Wrapper& sharedData, const std::string& key, const Json::Value& value, std::size_t index = 0) override;
        
        /// <summary>
        /// Completes handling of a json key value pair. If this method does inded handle the pair it returns true; otherwise false.
        /// </summary>
        /// <param name="sharedData">Wrapper to handle</param>
        /// <param name="key">Key to handle</param>
        /// <param name="value">Value to handle</param>
        /// <returns>True if the helper can parse this key value pair with this wrapper; otherwise false</returns>
        bool EndHandler(JsonParseCoordinator::Wrapper& sharedData, const std::string& key) override;
    
    private:

        struct ContextFrame
        {
            Datum& Datum;
            const std::string& DatumName;
        };

        std::string _GetClassNameFromTable(const Json::Value& value);
        void _PopulateDatumWithPrimitive(Datum& datum, const std::string& key, const Json::Value& value, bool isArray);
        void _SetDatumFromJsonValue(Datum& datum, const Json::Value& value, std::size_t index = 0);
        void _PushBackDatumFromJsonValue(Datum& datum, const Json::Value& value);
        std::stack<ContextFrame, Vector<ContextFrame>> _contextStack;
        std::unique_ptr<Scope> _tempWrapperScope;
    };
}
