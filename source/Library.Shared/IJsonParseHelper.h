#pragma once

#include "RTTI.h"
#include <json/json.h>
#include <memory>
#include "JsonParseCoordinator.h"

namespace FieaGameEngine
{
	class IJsonParseHelper : public RTTI
	{
		RTTI_DECLARATIONS(IJsonParseHelper, RTTI);

	public:
		virtual ~IJsonParseHelper() {};

		/// <summary>
		/// Creates a new instance of IJsonParseHelper polymorphically
		/// </summary>
		/// <returns></returns>
		[[nodiscard]] virtual std::unique_ptr<IJsonParseHelper> Create() const = 0;

		/// <summary>
		/// Initializes the helper right before deserializing
		/// </summary>
		virtual void Initialize(JsonParseCoordinator::Wrapper& sharedData);
		
		/// <summary>
		/// Performs a cleanup of initialized members after deserializing
		/// </summary>
		virtual void Cleanup(JsonParseCoordinator::Wrapper& sharedData);

		/// <summary>
		/// Tries to handle a json key value pair and populate the provided Wrapper.
		/// If the helper can handle this kind of data, then this method should return true; otherwise false. 
		/// </summary>
		/// <param name="sharedData">Wrapper to handle</param>
		/// <param name="key">Key to handle</param>
		/// <param name="value">Value to handle</param>
		/// <param name="index">Array index of this element</param>
		/// <returns>True if the helper can parse this key value pair with this wrapper; otherwise false</returns>
		virtual bool StartHandler(JsonParseCoordinator::Wrapper& sharedData, const std::string& key, const Json::Value& value, std::size_t index = 0) = 0;
		
		/// <summary>
		/// Completes handling of a json key value pair. If this method does inded handle the pair it returns true; otherwise false.
		/// </summary>
		/// <param name="sharedData">Wrapper to handle</param>
		/// <param name="key">Key to handle</param>
		/// <param name="value">Value to handle</param>
		/// <returns>True if the helper can parse this key value pair with this wrapper; otherwise false</returns>
		virtual bool EndHandler(JsonParseCoordinator::Wrapper& sharedData, const std::string& key) = 0;
	
		/// <summary>
		/// Compares two helpers
		/// <returns>True of equal; otherwise false</returns>
		/// </summary>
		[[nodiscard]] bool Equals(const RTTI* rhs) const override;
	};
}