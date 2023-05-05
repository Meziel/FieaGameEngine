#pragma once

#include <memory>
#include <sstream>
#include <json/json.h>
#include "Vector.h"
#include "RTTI.h"

namespace FieaGameEngine
{
	class IJsonParseHelper;

	/// <summary>
	/// Class that deserialized json into a data wrapper using helpers to perform
	/// the deserialization work
	/// </summary>
	class JsonParseCoordinator final
	{

	public:

		/// <summary>
		/// Abstract wrapper around data that all helpers will share with each other.
		/// The wrapper will be populated by the helpers in the JsonParseCoordinator.
		/// </summary>
		struct Wrapper : public RTTI
		{
			RTTI_DECLARATIONS(Wrapper, RTTI);

		public:
			/// <summary>
			/// Creates a new instance of this wrapper
			/// </summary>
			/// <returns>Newly created derived wrapper</returns>
			virtual std::unique_ptr<Wrapper> Create() const = 0;
			virtual ~Wrapper() = default;

			/// <summary>
			/// Method called before the wrapper is used to deserialize
			/// </summary>
			virtual void Initialize();

			/// <summary>
			/// Method called after the wrapper is done being used for deserialization
			/// </summary>
			virtual void Cleanup();

			/// <summary>
			/// Gets the depth in the json the parse coordinator is currently at.
			/// Depth is 0 indexed from the root.
			/// </summary>
			/// <returns>Depth level in the json</returns>
			[[nodiscard]] std::size_t Depth() const;

			/// <summary>
			/// Get the parse coordinator that is currently using this wrapper
			/// </summary>
			/// <returns>Parse coordinator using this wrapper</returns>
			[[nodiscard]] JsonParseCoordinator* GetJsonParseCoordinator();

			/// <summary>
			/// Get the parse coordinator that is currently using this wrapper
			/// </summary>
			/// <returns>Parse coordinator using this wrapper</returns>
			[[nodiscard]] const JsonParseCoordinator* GetJsonParseCoordinator() const;

		private:
			friend class JsonParseCoordinator;
			void SetJsonParseCoordinator(JsonParseCoordinator* jsonParseCoordinator);
			void IncrementDepth();
			void DecrementDepth();
			std::size_t _depth = 0;
			JsonParseCoordinator* _jsonParseCoordinator = nullptr;
		};
		
		/// <summary>
		/// Created a JsonParseCoordinator with a given wrapper to populate
		/// </summary>
		/// <param name="wrapper">Wrapper surrounding data that should be populated when the json is deserialized</param>
		explicit JsonParseCoordinator(std::shared_ptr<Wrapper> wrapper);
		JsonParseCoordinator(const JsonParseCoordinator&) = delete;
		JsonParseCoordinator(JsonParseCoordinator&& rhs) noexcept;
		~JsonParseCoordinator() = default;
		JsonParseCoordinator& operator=(const JsonParseCoordinator&) = delete;
		JsonParseCoordinator& operator=(JsonParseCoordinator&& rhs) noexcept;

		/// <summary>
		/// Creates a copy of a JsonParseCoordinator, creating new instances of helpers and wrappers.
		/// The new helper and wrapper states are not copied from the JsonParseCoordinator.
		/// </summary>
		/// <returns>Copy of a JsonParseCoordinator</returns>
		[[nodiscard]] std::unique_ptr<JsonParseCoordinator> Clone() const;

		/// <summary>
		/// Adds a helper to the ParseCoordinator.
		/// </summary>
		/// <param name="helper">Helper to add</param>
		void AddHelper(std::shared_ptr<IJsonParseHelper> helper);

		/// <summary>
		/// Remove a helper from the ParseCoordinator.
		/// </summary>
		/// <param name="helper">Helper to remove</param>
		void RemoveHelper(std::shared_ptr<IJsonParseHelper> helper);

		/// <summary>
		/// List of helpers that have been added to the ParseCoordinator
		/// </summary>
		/// <returns>List of helpers</returns>
		[[nodiscard]] Vector<std::shared_ptr<IJsonParseHelper>>& Helpers();

		/// <summary>
		/// List of helpers that have been added to the ParseCoordinator
		/// </summary>
		/// <returns>List of helpers</returns>
		const Vector<std::shared_ptr<IJsonParseHelper>>& Helpers() const;

		/// <summary>
		/// Deserializes a json string using the helpers added to this object
		/// Will populate the wrapper if the parse coordinator can deserialize the data
		/// </summary>
		/// <param name="json">Json string to parse</param>
		void DeserializeObject(const std::string& json);
		
		/// <summary>
		/// Deserializes a json file using the helpers added to this object
		/// Will populate the wrapper if the parse coordinator can deserialize the data
		/// </summary>
		/// <param name="file">Json file name to parse</param>
		void DeserializeObjectFromFile(const std::string& file);
		
		/// <summary>
		/// Deserializes a json stream using the helpers added to this object
		/// Will populate the wrapper if the parse coordinator can deserialize the data
		/// </summary>
		/// <param name="json">Stream containing json to parse</param>
		void DeserializeObject(std::istream& json);

		/// <summary>
		/// Gets the wrapper set on this ParseCoordinator
		/// </summary>
		/// <returns>Wrapper for this ParseCoordinator</returns>
		[[nodiscard]] Wrapper& GetWrapper();

		/// <summary>
		/// Sets the wrapper for this ParseCoordinator
		/// </summary>
		/// <param name="wrapper">Wrapper to set</param>
		void SetWrapper(std::shared_ptr<Wrapper> wrapper);

	private:
		Vector<std::shared_ptr<IJsonParseHelper>> _helpers;
		std::shared_ptr<Wrapper> _wrapper;

		void ParseMembers(const Json::Value& value, std::size_t arrayIndex = 0);
		void ParseArray(const std::string& key, const Json::Value& value, std::size_t arrayIndex = 0);
		void ParseKeyValuePair(const std::string& key, const Json::Value& value, std::size_t arrayIndex = 0);
	};
}