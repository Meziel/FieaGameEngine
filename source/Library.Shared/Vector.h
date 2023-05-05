#pragma once

#include <cstddef>
#include <memory>
#include <memory_resource>
#include <functional>
#include "DefaultEquality.h"
#include "DefaultReserveStrategy.h"

namespace FieaGameEngine
{
	/// <summary>
	/// Randomly accessible dynamic array
	/// </summary>
	/// <typeparam name="T"></typeparam>
	template <typename T>
	class Vector final
	{
	public:
		using size_type = std::size_t;
		using value_type = T;
		using reference = value_type&;
		using const_reference = const value_type&;
		using rvalue_reference = value_type&&;
		using pointer = value_type*;
		using const_pointer = const value_type*;
		using ReserveStrategy = std::function<std::size_t(std::size_t)>;
		using EqualityFunctor = std::function<bool(const_reference lhs, const_reference rhs)>;
		using Allocator = std::pmr::polymorphic_allocator<T>;
		// std library redirects
		inline bool empty() const { return IsEmpty(); };
		inline size_t size() const { return Size(); };
		inline const_reference back() const { return Back(); };
		inline reference back() { return Back(); };
		inline void pop_back() { PopBack(); };
		inline void push_back(const_reference v) { PushBack(v); };
		inline void push_back(reference v) { PushBack(v); };

	public:
		class iterator;

		/// <summary>
		/// Iterator that when dereferenced returns a const version of a value in the container
		/// </summary>
		class const_iterator final
		{
			friend class Vector;

		public:
			using iterator_category = std::contiguous_iterator_tag;
			using difference_type = std::ptrdiff_t;
			using value_type = value_type;
			using pointer = value_type*;
			using reference = value_type&;

			const_iterator() = default;
			const_iterator& operator=(const const_iterator&) = default;

			/// <summary>
			/// Creates a constant iterator from a non-const iterator
			/// </summary>
			/// <param name="other">iterator to turn into a const_iterator</param>
			const_iterator(const iterator& other);

			/// <summary>
			/// Gets a non-const version of the value in the container pointed to by this iterator
			/// </summary>
			/// <exception cref="runtime_error">If the iterator is not associated with a container or points to an element not within a container</exception>
			/// <returns>value in the container</returns>
			[[nodiscard]] const_reference operator*() const;

			/// <summary>
			/// Gets a non-const version of the value in the container pointed to by this iterator
			/// </summary>
			/// <exception cref="runtime_error">If the iterator is not associated with a container or points to an element not within a container</exception>
			/// <returns>value in the container</returns>
			[[nodiscard]] const value_type* operator->() const;

			/// <summary>
			/// Gets a value in the container at the given index
			/// </summary>
			/// <exception cref="runtime_error">If the iterator is not associated with a container or index is not within the bounds of the container</exception>
			/// <returns>value in the container at the given index</returns>
			[[nodiscard]] const_reference operator[](std::size_t index) const;

			/// <summary>
			/// Increments the iterator so the iterator points to the next element in the container
			/// </summary>
			/// <exception cref="runtime_error">Iterator is not associated with a container</exception>
			/// <returns>Reference to the incremented iterator</returns>
			const_iterator& operator++();

			/// <summary>
			/// Increments the iterator so the iterator points to the next element in the container
			/// </summary>
			/// <param name=""></param>
			/// <exception cref="runtime_error">Iterator is not associated with a container</exception>
			/// <returns>Iterator before it is incremented</returns>
			const_iterator operator++(int);

			/// <summary>
			/// Decrements the iterator so the iterator points to the previous element in the container
			/// </summary>
			/// <exception cref="runtime_error">Iterator is not associated with a container</exception>
			/// <returns>Reference to the decremented iterator</returns>
			const_iterator& operator--();

			/// <summary>
			/// Decrements the iterator so the iterator points to the previous element in the container
			/// </summary>
			/// <exception cref="runtime_error">Iterator is not associated with a container</exception>
			/// <returns>Iterator before it is decremented</returns>
			const_iterator operator--(int);

			/// <summary>
			/// Increments the iterator so the iterator points to nth element ahead from its current position in the container
			/// </summary>
			/// <param name="n">Positions to increment by</param>
			/// <exception cref="runtime_error">Iterator is not associated with a container</exception>
			/// <returns>Reference to the incremented iterator</returns>
			const_iterator& operator+=(size_t n);

			/// <summary>
			/// Adds a number to the iterator and returns a new iterator to nth element ahead from the current iterator's position in the container
			/// </summary>
			/// <exception cref="runtime_error">Iterator is not associated with a container</exception>
			/// <returns>Iterator to current position + n</returns>
			[[nodiscard]] const_iterator operator+(size_t n) const;

			/// <summary>
			/// Decrements the iterator so the iterator points to nth element behind from its current position in the container
			/// </summary>
			/// <param name="n">Positions to decrement by</param>
			/// <exception cref="runtime_error">Iterator is not associated with a container</exception>
			/// <returns>Reference to the decremented iterator</returns>
			const_iterator& operator-=(size_t n);

			/// <summary>
			/// Subtracts a number to the iterator and returns a new iterator to nth element behind from the current iterator's position in the container
			/// </summary>
			/// <param name="n">Positions to decrement by</param>
			/// <exception cref="runtime_error">Iterator is not associated with a container</exception>
			/// <returns>Iterator to current position - n</returns>
			[[nodiscard]] const_iterator operator-(size_t n) const;

			/// <summary>
			/// Subtracts two iterators of the same container and returns their difference in position.
			/// Iterator to subtract should be before the current position or at the current position of this iterator
			/// </summary>
			/// <param name="it">Iterator to subtract</param>
			/// <exception cref="runtime_error">Iterators are from different containers</exception>
			/// <returns>Difference in positions between the two iterators</returns>
			[[nodiscard]] size_t operator-(const const_iterator& it) const;

			/// <summary>
			/// Checks if the current iterators position is less than an iterator in the same container
			/// </summary>
			/// <param name="it">Iterator to compare against</param>
			/// <exception cref="runtime_error">Iterators are from different containers</exception>
			/// <returns>True if current iterators position is less than the provided iterator; otherwise false</returns>
			[[nodiscard]] bool operator<(const const_iterator& it) const;

			/// <summary>
			/// Checks if the current iterators position is less or equal to an iterator in the same container
			/// </summary>
			/// <param name="it">Iterator to compare against</param>
			/// <exception cref="runtime_error">Iterators are from different containers</exception>
			/// <returns>True if current iterators position is less or equal to the provided iterator; otherwise false</returns>
			[[nodiscard]] bool operator<=(const const_iterator& it) const;

			/// <summary>
			/// Checks if the current iterators position is greater than an iterator in the same container
			/// </summary>
			/// <param name="it">Iterator to compare against</param>
			/// <exception cref="runtime_error">Iterators are from different containers</exception>
			/// <returns>True if current iterators position is greater than the provided iterator; otherwise false</returns>
			[[nodiscard]] bool operator>(const const_iterator& it) const;

			/// <summary>
			/// Checks if the current iterators position is greater than or equal to an iterator in the same container
			/// </summary>
			/// <param name="it">Iterator to compare against</param>
			/// <exception cref="runtime_error">Iterators are from different containers</exception>
			/// <returns>True if current iterators position is greater than or equal to the provided iterator; otherwise false</returns>
			[[nodiscard]] bool operator>=(const const_iterator& it) const;

			/// <summary>
			/// Compares iterators. If iterators are from different containers, returns false;
			/// otherwise iterators are equal if there positions are equal.
			/// Comparing between two default constructed iterators always returns true.
			/// </summary>
			/// <param name="other"></param>
			/// <returns>Whether the iterators are equal</returns>
			[[nodiscard]] bool operator==(const const_iterator& other) const;

			/// <summary>
			/// Compares iterators. If iterators are from different containers, returns true;
			/// otherwise iterators are equal if there positions are equal.
			/// Comparing between two default constructed iterators always returns false. 
			/// </summary>
			/// <param name="other"></param>
			/// <returns>Whether the iterators are not equal</returns>
			[[nodiscard]] bool operator!=(const const_iterator& other) const;

		private:
			/// <summary>
			/// Creates an iterator associated with a container and points to a given node 
			/// </summary>
			/// <param name="container">Container the iterator is associated with</param>
			/// <param name="node">Node the iterator points to</param>
			const_iterator(const Vector& container, pointer ptr = nullptr);

			pointer _ptr = nullptr;
			const Vector* _container = nullptr;
		};

		/// <summary>
		/// Iterator that when dereferenced returns a non-const version of a value in the container
		/// </summary>
		class iterator final
		{
			friend Vector;
			friend const_iterator;

		public:
			using iterator_category = std::contiguous_iterator_tag;
			using difference_type = std::ptrdiff_t;
			using value_type = value_type;
			using pointer = value_type*;
			using reference = value_type&;

			iterator() = default;
			iterator& operator=(const iterator&) = default;

			/// <summary>
			/// Gets a the value in the container pointed to by this iterator
			/// </summary>
			/// <exception cref="runtime_error">If the iterator is not associated with a container or points to an element not within a container</exception>
			/// <returns>value in the container</returns>
			[[nodiscard]] reference operator*() const;

			/// <summary>
			/// Gets a the value in the container pointed to by this iterator
			/// </summary>
			/// <exception cref="runtime_error">If the iterator is not associated with a container or points to an element not within a container</exception>
			/// <returns>value in the container</returns>
			[[nodiscard]] pointer operator->() const;

			/// <summary>
			/// Gets a value in the container at the given index
			/// </summary>
			/// <exception cref="runtime_error">If the iterator is not associated with a container or index is not within the bounds of the container</exception>
			/// <returns>value in the container at the given index</returns>
			[[nodiscard]] reference operator[](size_t index) const;

			/// <summary>
			/// Increments the iterator so the iterator points to the next element in the container
			/// </summary>
			/// <exception cref="runtime_error">Iterator is not associated with a container</exception>
			/// <returns>Reference to the incremented iterator</returns>
			iterator& operator++();

			/// <summary>
			/// Increments the iterator so the iterator points to the next element in the container
			/// </summary>
			/// <exception cref="runtime_error">Iterator is not associated with a container</exception>
			/// <returns>Iterator before it is incremented</returns>
			iterator operator++(int);

			/// <summary>
			/// Decrements the iterator so the iterator points to the previous element in the container
			/// </summary>
			/// <exception cref="runtime_error">Iterator is not associated with a container</exception>
			/// <returns>Reference to the decremented iterator</returns>
			iterator& operator--();

			/// <summary>
			/// Decrements the iterator so the iterator points to the previous element in the container
			/// </summary>
			/// <exception cref="runtime_error">Iterator is not associated with a container</exception>
			/// <returns>Iterator before it is decremented</returns>
			iterator operator--(int);

			/// <summary>
			/// Increments the iterator so the iterator points to nth element ahead from its current position in the container
			/// </summary>
			/// <param name="n">Positions to increment by</param>
			/// <exception cref="runtime_error">Iterator is not associated with a container</exception>
			/// <returns>Reference to the incremented iterator</returns>
			iterator& operator+=(size_t n);

			/// <summary>
			/// Adds a number to the iterator and returns a new iterator to nth element ahead from the current iterator's position in the container
			/// </summary>
			/// <exception cref="runtime_error">Iterator is not associated with a container</exception>
			/// <returns>Iterator to current position + n</returns>
			[[nodiscard]] iterator operator+(size_t n) const;

			/// <summary>
			/// Decrements the iterator so the iterator points to nth element behind from its current position in the container
			/// </summary>
			/// <param name="n">Positions to decrement by</param>
			/// <exception cref="runtime_error">Iterator is not associated with a container</exception>
			/// <returns>Reference to the decremented iterator</returns>
			iterator& operator-=(size_t n);

			/// <summary>
			/// Subtracts a number to the iterator and returns a new iterator to nth element behind from the current iterator's position in the container
			/// </summary>
			/// <param name="n">Positions to decrement by</param>
			/// <exception cref="runtime_error">Iterator is not associated with a container</exception>
			/// <returns>Iterator to current position - n</returns>
			[[nodiscard]] iterator operator-(size_t n) const;

			/// <summary>
			/// Subtracts two iterators of the same container and returns their difference in position.
			/// Iterator to subtract should be before the current position or at the current position of this iterator
			/// </summary>
			/// <param name="it">Iterator to subtract</param>
			/// <exception cref="runtime_error">Iterators are from different containers</exception>
			/// <returns>Difference in positions between the two iterators</returns>
			[[nodiscard]] size_t operator-(const iterator& it) const;

			/// <summary>
			/// Checks if the current iterators position is less than an iterator in the same container
			/// </summary>
			/// <param name="it">Iterator to compare against</param>
			/// <exception cref="runtime_error">Iterators are from different containers</exception>
			/// <returns>True if current iterators position is less than the provided iterator; otherwise false</returns>
			[[nodiscard]] bool operator<(const iterator& it) const;

			/// <summary>
			/// Checks if the current iterators position is less or equal to an iterator in the same container
			/// </summary>
			/// <param name="it">Iterator to compare against</param>
			/// <exception cref="runtime_error">Iterators are from different containers</exception>
			/// <returns>True if current iterators position is less or equal to the provided iterator; otherwise false</returns>
			[[nodiscard]] bool operator<=(const iterator& it) const;

			/// <summary>
			/// Checks if the current iterators position is greater than an iterator in the same container
			/// </summary>
			/// <param name="it">Iterator to compare against</param>
			/// <exception cref="runtime_error">Iterators are from different containers</exception>
			/// <returns>True if current iterators position is greater than the provided iterator; otherwise false</returns>
			[[nodiscard]] bool operator>(const iterator& it) const;

			/// <summary>
			/// Checks if the current iterators position is greater than or equal to an iterator in the same container
			/// </summary>
			/// <param name="it">Iterator to compare against</param>
			/// <exception cref="runtime_error">Iterators are from different containers</exception>
			/// <returns>True if current iterators position is greater than or equal to the provided iterator; otherwise false</returns>
			[[nodiscard]] bool operator>=(const iterator& it) const;

			/// <summary>
			/// Compares iterators. If iterators are from different containers, returns false;
			/// otherwise iterators are equal if there positions are equal.
			/// Comparing between two default constructed iterators always returns true.
			/// </summary>
			/// <param name="other"></param>
			/// <returns>Whether the iterators are equal</returns>
			[[nodiscard]] bool operator==(const iterator& other) const;

			/// <summary>
			/// Compares iterators. If iterators are from different containers, returns true;
			/// otherwise iterators are equal if there positions are equal.
			/// Comparing between two default constructed iterators always returns false. 
			/// </summary>
			/// <param name="other"></param>
			/// <returns>Whether the iterators are not equal</returns>
			[[nodiscard]] bool operator!=(const iterator& other) const;

		private:
			/// <summary>
			/// Creates an iterator associated with a container and points to a given node 
			/// </summary>
			/// <param name="container">Container the iterator is associated with</param>
			/// <param name="node">Node the iterator points to</param>
			iterator(Vector& container, pointer ptr = nullptr);

			pointer _ptr = nullptr;
			Vector* _container = nullptr;
		};

		/// <summary>
		/// Constructs a vector
		/// </summary>
		/// <param name="capacity">Default capacity</param>
		/// <param name="reserveStrategy">Functor the increases the capacity when the the vector needs to grow.</param>
		Vector(std::size_t capacity = 0, ReserveStrategy reserveStrategy = DefaultReserveStrategy{});

		/// <summary>
		/// Creates a vector from an initializer list of values
		/// </summary>
		/// <param name="list">initializer list of values</param>
		Vector(std::initializer_list<value_type> list);

		/// <summary>
		/// Copy constructor
		/// </summary>
		Vector(const Vector& other);

		/// <summary>
		/// Move constructor
		/// </summary>
		Vector(Vector&& other) noexcept;

		/// <summary>
		/// Copy assignment
		/// </summary>
		Vector& operator=(const Vector& rhs);

		/// <summary>
		/// Move assignment
		/// </summary>
		Vector& operator=(Vector&& rhs) noexcept;

		/// <summary>
		/// Destructor
		/// </summary>
		~Vector();

		/// <summary>
		/// Gets an iterator pointing to the first element in the container
		/// </summary>
		/// <returns>iterator at the beginning of the container</returns>
		[[nodiscard]] iterator begin();

		/// <summary>
		/// Gets a const_iterator pointing to the first element in the container
		/// </summary>
		/// <returns>const_iterator at the beginning of the container</returns>
		[[nodiscard]] const_iterator begin() const;

		/// <summary>
		/// Gets an iterator pointing to one past the last element in the container
		/// </summary>
		/// <returns>iterator at the end of the container</returns>
		[[nodiscard]] iterator end();

		/// <summary>
		/// Gets a const_iterator pointing to one past the last element in the container
		/// </summary>
		/// <returns>iterator at the end of the container</returns>
		[[nodiscard]] const_iterator end() const;

		/// <summary>
		/// Gets a const_iterator pointing to the first element in the container
		/// </summary>
		/// <returns>const_iterator at the beginning of the container</returns>
		[[nodiscard]] const_iterator cbegin() const;

		/// <summary>
		/// Gets a const_iterator pointing to one past the last element in the container
		/// </summary>
		/// <returns>const_iterator at the end of the container</returns>
		[[nodiscard]] const_iterator cend() const;

		/// <summary>
		/// Gets the element at the given index
		/// </summary>
		/// <param name="index">position to randomly access in the vector</param>
		/// <exception cref="runtime_error">Index out of bounds</exception>
		/// <returns>value at the given index</returns>
		[[nodiscard]] reference operator[](std::size_t index);

		/// <summary>
		/// Gets the element at the given index
		/// </summary>
		/// <param name="index">position to randomly access in the vector</param>
		/// <exception cref="runtime_error">Index out of bounds</exception>
		/// <returns>value at the given index</returns>
		[[nodiscard]] const_reference operator[](std::size_t index) const;

		/// <summary>
		/// Gets the element at the given index
		/// </summary>
		/// <param name="index">position to randomly access in the vector</param>
		/// <exception cref="runtime_error">Index out of bounds</exception>
		/// <returns>value at the given index</returns>
		[[nodiscard]] reference At(std::size_t index);

		/// <summary>
		/// Gets the element at the given index
		/// </summary>
		/// <param name="index">position to randomly access in the vector</param>
		/// <exception cref="runtime_error">Index out of bounds</exception>
		/// <returns>value at the given index</returns>
		[[nodiscard]] const_reference At(std::size_t index) const;

		/// <summary>
		/// Removes an element from the back of the container.
		/// <exception cref="runtime_error">Vector is empty</exception>
		/// </summary>
		void PopBack();

		/// <summary>
		/// Returns whether the container is empty
		/// </summary>
		/// <returns>True of the container is empty; otherwise false</returns>
		[[nodiscard]] bool IsEmpty() const;

		/// <summary>
		/// Return the first item in the container
		/// </summary>
		/// <exception cref="runtime_error">Vector is empty</exception>
		/// <returns>Front value</returns>
		[[nodiscard]] reference Front();

		/// <summary>
		/// Return the first item in the container
		/// </summary>
		/// <exception cref="runtime_error">Vector is empty</exception>
		/// <returns>Font value</returns>
		[[nodiscard]] const_reference Front() const;

		/// <summary>
		/// Return the last item in the container
		/// </summary>
		/// <exception cref="runtime_error">Vector is empty</exception>
		/// <returns>Back value</returns>
		[[nodiscard]] reference Back();

		/// <summary>
		/// Return the last item in the container
		/// </summary>
		/// <exception cref="runtime_error">Vector is empty</exception>
		/// <returns>Back value</returns>
		[[nodiscard]] const_reference Back() const;

		/// <summary>
		/// Return the number of items in the container
		/// </summary>
		/// <returns>Size</returns>
		[[nodiscard]] std::size_t Size() const;

		/// <summary>
		/// Returns the number of items allocated within the container
		/// </summary>
		/// <returns>Capacity</returns>
		[[nodiscard]] std::size_t Capacity() const;

		/// <summary>
		/// Reserves space in the container
		/// </summary>
		/// <param name="capacity">Number of elements to reserve</param>
		void Reserve(size_t capacity);

		void Resize(size_t size);
		void Resize(size_t size, const_reference value);

		/// <summary>
		/// Adds an element to the back of the container
		/// </summary>
		/// <param name="value">Value to add</param>
		/// <returns>Iterator pointing to the value inserted</returns>
		iterator PushBack(const_reference value);

		/// <summary>
		/// Adds an element to the back of the container
		/// </summary>
		/// <param name="value">Value to add. The value is made unusable after calling this method</param>
		/// <returns>Iterator pointing to the value inserted</returns>
		iterator PushBack(rvalue_reference value);

		/// <summary>
		/// Creates a new element and adds it to the back of the container. Constructs the element from a list of arguments
		/// </summary>
		/// <typeparam name="...Args"></typeparam>
		/// <param name="...args">Arguments used to construct the element</param>
		/// <returns>Iterator pointing to the value inserted</returns>
		template <typename... Args>
		iterator EmplaceBack(Args... args);

		Vector& Append(const Vector& other);

		/// <summary>
		/// Finds a element in the containers and returns an iterator to the element.
		/// If no element is found, an iterator pointing to the end of the container is returned.
		/// </summary>
		/// <param name="value">Value to find</param>
		/// <returns>Iterator pointing to the value found or to the end of the container</returns>
		iterator Find(const const_reference value, EqualityFunctor equalityFunctor = DefaultEquality<T>{});

		/// <summary>
		/// Finds a element in the containers and returns an iterator to the element.
		/// If no element is found, an iterator pointing to the end of the container is returned.
		/// </summary>
		/// <param name="value">Value to find</param>
		/// <returns>Iterator pointing to the value found or to the end of the container</returns>
		const_iterator Find(const const_reference value, EqualityFunctor equalityFunctor = DefaultEquality<T>{}) const;

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
		/// Removes the first instance of an element from the container
		/// </summary>
		/// <param name="value">element to remove</param>
		/// <returns>True if the element was found; otherwise false</returns>
		bool Remove(const_reference value, EqualityFunctor equalityFunctor = DefaultEquality<T>{});

		/// <summary>
		/// Removes the element pointed to by the given interator.
		/// If the iterator is not from this container or the iterator is out of bounds,
		/// nothing is removed and the method returns false
		/// </summary>
		/// <param name="first"></param>
		/// <returns>True if an element was removed; otherwise false</returns>
		bool Remove(const iterator& first);

		/// <summary>
		/// Removes the elements between the range [first, last).
		/// If the iterators are not from this container or the iterators are out of bounds,
		/// nothing is removed and the method returns false
		/// </summary>
		/// <param name="first">iterator pointing to the position to start removing</param>
		/// <param name="first">fiterator pointing to the position to stop removing</param>
		/// <returns>True if an element was removed; otherwise false</returns>
		bool Remove(const iterator& first, const iterator& last);

	private:
		Allocator _alloc;
		value_type* _data = nullptr;
		std::size_t _capacity = 0;
		std::size_t _size = 0;
		ReserveStrategy _reserveStrategy = DefaultReserveStrategy{};
	};
}
#include <Vector>

#include "Vector.inl"