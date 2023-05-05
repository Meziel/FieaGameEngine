#pragma once

#include <cstddef>
#include <type_traits>
#include <functional>
#include <initializer_list>
#include <string>
#pragma warning(push)
#pragma warning(disable:4201)
#include <glm/gtx/string_cast.hpp>
#pragma warning(pop)
#include "DatumTypes.h"

namespace FieaGameEngine
{
	/// <summary>
	/// Type that requires a ToDatumType() specialization
	/// </summary>
	template<class T>
	concept VariantDatumType = requires(T)
	{
		ToDatumType<T>();
	};

	/// <summary>
	/// Custom std::variant that uses dynamically allocated vectors
	/// Can also point to external data
	/// </summary>
	/// <typeparam name="...Types">Variant types</typeparam>
	template <VariantDatumType... Types>
	class VariantDatum
	{

	public:

		/// <summary>
		/// Creates a datum with a specific type
		/// </summary>
		/// <param name="type">Type to set the datum</param>
		explicit VariantDatum(DatumTypes type = DatumTypes::Unknown);
		
		/// <summary>
		/// Creates a datum from a single value. Sets datum as internal
		/// </summary>
		/// <param name="value">Value to insert into the datum</param>
		template <VariantDatumType T>
		VariantDatum(const T& value);

		/// <summary>
		/// Creates a datum from a single value. Sets datum as internal
		/// </summary>
		/// <param name="value">Value to insert into the datum</param>
		template <VariantDatumType T>
		VariantDatum(T&& value);

		/// <summary>
		/// Creates a datum from another datum
		/// </summary>
		/// <param name="other">Datum</param>
		VariantDatum(const VariantDatum& other);

		/// <summary>
		/// Creates a datum from another datum by stealing its data
		/// </summary>
		/// <param name="other"></param>
		/// <returns>Temporary datum</returns>
		VariantDatum(VariantDatum&& other) noexcept;

		/// <summary>
		/// Creates a datum from an external array
		/// </summary>
		/// <param name="data">External array</param>
		/// <param name="size">Size of the external array</param>
		template <VariantDatumType T>
		VariantDatum(T* data, std::size_t size);

		/// <summary>
		/// Creates a datum from a list of values. Sets the datum as interal.
		/// </summary>
		/// <param name="list">Values to insert into the datum</param>
		template <VariantDatumType T>
		VariantDatum(std::initializer_list<T> list);

		virtual ~VariantDatum();

		/// <summary>
		/// Assignment from another datum
		/// </summary>
		VariantDatum& operator=(const VariantDatum& rhs);

		/// <summary>
		/// Assignment from an rvalue datum
		/// </summary>
		VariantDatum& operator=(VariantDatum&& rhs) noexcept;
		
		/// <summary>
		/// Assignment from a single value. Datum will be a size
		/// of 1 and be internal
		/// </summary>
		/// <param name="rhs">Value to insert into datum</param>
		template <VariantDatumType T>
		VariantDatum& operator=(const T& rhs);

		/// <summary>
		/// Assignment from a single rvalue value. Datum will be a size
		/// of 1 and be internal
		/// </summary>
		/// <param name="rhs">Value to insert into datum</param>
		template <VariantDatumType T>
		VariantDatum& operator=(T&& rhs);

		/// <summary>
		/// Gets the type of the datum
		/// </summary>
		[[nodiscard]] DatumTypes Type() const;

		/// <summary>
		/// Sets the type of the datum
		/// </summary>
		/// <param name="type">Datum type to set</param>
		void SetType(DatumTypes type);

		/// <summary>
		/// Gets the number of element in the container
		/// </summary>
		/// <returns>Number of elements</returns>
		[[nodiscard]] std::size_t Size() const;

		/// <summary>
		/// Gets the capacity of the container
		/// </summary>
		/// <returns>Capacity</returns>
		[[nodiscard]] std::size_t Capacity() const;

		/// <summary>
		/// Gets the whether the container is empty
		/// </summary>
		/// <returns>True if the container is empty; otherwise false</returns>
		[[nodiscard]] bool IsEmpty() const;

		/// <summary>
		/// Reserves space in the container
		/// </summary>
		/// <param name="capacity">Capacity to set to</param>
		void Reserve(std::size_t capacity);

		/// <summary>
		/// Sets the size and capacity of the container to the given size. Reserves memory if needed.
		/// This allows shrinking and growing. After Resize, size and capacity will be the same
		/// the datum's type.
		/// If growing, the datum will be populated with default constructed objects of the
		/// type of this datum.
		/// </summary>
		/// <param name="size">Space to reserve and fill</param>
		void Resize(std::size_t size);

		/// <summary>
		/// Sets the size and capacity of the container to the given size. Reserves memory if needed.
		/// This allows shrinking and growing. After Resize, size and capacity will be the same.
		/// If growing, the datum will be populated with values copied from the provided prototype.
		/// </summary>
		/// <param name="size">Size to set the container to</param>
		/// <param name="value">Prototype used to fill the container</param>
		template <VariantDatumType T>
		void Resize(std::size_t size, const T& value);

		/// <summary>
		/// Clears the elements of the container.
		/// Decreases the size of the container to 0.
		/// </summary>
		void Clear();

		/// <summary>
		/// Shrinks the capacity of the container to match the size
		/// </summary>
		void ShrinkToFit();

		/// <summary>
		/// Sets the datum to use external storage
		/// </summary>
		/// <param name="data">Pointer to external storage</param>
		/// <param name="size">Size of the external data</param>
		template <VariantDatumType T>
		void SetStorage(T* data, std::size_t size);

		/// <summary>
		/// Whether the data stored in the datum is external
		/// </summary>
		/// <returns>True if external; otherwise False</returns>
		bool IsExternal();

		/// <summary>
		/// Compares with another datum
		/// </summary>
		/// <param name="rhs">Datum</param>
		[[nodiscard]] bool operator==(const VariantDatum& rhs) const;

		/// <summary>
		/// Compares with another datum
		/// </summary>
		/// <param name="rhs">Datum</param>
		[[nodiscard]] bool operator!=(const VariantDatum& rhs) const;

		/// <summary>
		/// Compares with a single datum supported value type
		/// </summary>
		/// <param name="rhs">Datum supported value type to compare with</param>
		template <VariantDatumType T>
		[[nodiscard]] bool operator==(const T& rhs) const;

		/// <summary>
		/// Compares with a single datum supported value type
		/// </summary>
		/// <param name="rhs">Datum supported value type to compare with</param>
		template <VariantDatumType T>
		[[nodiscard]] bool operator!=(const T& rhs) const;

		/// <summary>
		/// Sets the a datum supported value at the given index
		/// </summary>
		/// <param name="value">Datum supported value to set</param>
		/// <param name="size">Index to set in the value</param>
		template <VariantDatumType T>
		void Set(const T& value, std::size_t size = 0);

		/// <summary>
		/// Sets the a datum supported rvalue at the given index
		/// </summary>
		/// <param name="value">Datum supported value to set</param>
		/// <param name="size">Index to set in the value</param>
		template <VariantDatumType T>
		void Set(T&& value, std::size_t size = 0);

		/// <summary>
		/// Gets a datum supported value at the given index
		/// </summary>
		/// <param name="size">Index to get from the container</param>
		/// <returns>Value retrieved from the container at the index</returns>
		template <VariantDatumType T>
		[[nodiscard]] T& Get(std::size_t index = 0);

		/// <summary>
		/// Gets a datum supported value at the given index
		/// </summary>
		/// <param name="size">Index to get from the container</param>
		/// <returns>Value retrieved from the container at the index</returns>
		template <VariantDatumType T>
		[[nodiscard]] const T& Get(std::size_t index = 0) const;

		/// <summary>
		/// Gets a datum supported value at the given index
		/// </summary>
		/// <param name="size">Index to get from the container</param>
		/// <returns>Value retrieved from the container at the index</returns>
		template <VariantDatumType T>
		[[nodiscard]] const T& operator[](std::size_t index) const;

		/// <summary>
		/// Adds an element to the back of the container
		/// </summary>
		/// <param name="value">Value to add</param>
		template <VariantDatumType T>
		requires std::copy_constructible<T>
		void PushBack(const T& value);

		/// <summary>
		/// Adds an element to the back of the container
		/// </summary>
		/// <param name="value">Value to add. The value is made unusable after calling this method</param>
		template <VariantDatumType T>
		void PushBack(T&& value);

		/// <summary>
		/// Creates a new element and adds it to the back of the container. Constructs the element from a list of arguments
		/// </summary>
		/// <typeparam name="...Args"></typeparam>
		/// <param name="...args">Arguments used to construct the element</param>
		template <VariantDatumType T, typename... Args>
		void EmplaceBack(Args... args);

		/// <summary>
		/// Removes an element from the back of the container.
		/// </summary>
		void PopBack();

		/// <summary>
		/// Return the first item in the container
		/// </summary>
		/// <returns>Front value</returns>
		template <VariantDatumType T>
		[[nodiscard]] T& Front();

		/// <summary>
		/// Return the first item in the container
		/// </summary>
		/// <exception cref="runtime_error">Vector is empty</exception>
		/// <returns>Font value</returns>
		template <VariantDatumType T>
		[[nodiscard]] const T& Front() const;

		/// <summary>
		/// Return the last item in the container
		/// </summary>
		/// <returns>Back value</returns>
		template <VariantDatumType T>
		[[nodiscard]] T& Back();

		/// <summary>
		/// Return the last item in the container
		/// </summary>
		/// <returns>Back value</returns>
		template <VariantDatumType T>
		[[nodiscard]] const T& Back() const;

		// <summary>
		/// Removes the first instance of an element from the container
		/// </summary>
		/// <param name="value">element to remove</param>
		/// <returns>True if the element was found; otherwise false</return>
		template <VariantDatumType T>
		bool Remove(const T& value);

		// <summary>
		/// Removes the element at the given index
		/// </summary>
		/// <param name="index">Index to remove from the container</param>
		/// <returns>True if the element at the index was removed; otherwise false</return>
		bool RemoveAt(std::size_t index = 0);

		/// <summary>
		/// Finds a element in the container and returns an index where the element is in the container.
		/// If no element is found, the size of the container is returned.
		/// </summary>
		/// <param name="value">Value to find</param>
		/// <returns>Index of the value found if found; otherwise the size of the container</returns>
		template <VariantDatumType T>
		std::size_t Find(const T& value);

	private:

		std::byte* _data = nullptr;
		std::size_t _size = 0;
		std::size_t _capacity = 0;
		DatumTypes _type = DatumTypes::Unknown;
		bool _isExternal = false;

		friend class Attributed;
		void SetStorage(DatumTypes type, void* data, std::size_t size);

		template <typename Func, typename... Args>
		inline void Visit(Func func, Args&&...args);

		template <typename Func, typename... Args>
		inline void Visit(const Func func, Args&& ...args) const;
	};

	class Datum final : public VariantDatum<bool, int32_t, float, std::string, glm::vec4, glm::mat4, RTTI*, Scope*, std::shared_ptr<Scope>>
	{
	public:
		using VariantDatum::VariantDatum;
		using VariantDatum::operator=;

		/// <summary>
		/// Gets a scope at the given index
		/// </summary>
		/// <param name="size">Index to get from the container</param>
		/// <returns>Scope retrieved from the container at the index</returns>
		[[nodiscard]] Scope& operator[](std::size_t index);
	};
}

#include "Datum.inl"