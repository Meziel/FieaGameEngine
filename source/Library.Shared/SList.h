#pragma once

#include <cstddef>
#include <initializer_list>
#include <functional>
#include <memory>
#include <memory_resource>
#include "DefaultEquality.h"

namespace FieaGameEngine
{
	/// <summary>
	/// Forward linked list
	/// </summary>
	/// <typeparam name="T"></typeparam>
	template <typename T>
	class SList final
	{
	public:
		struct Node;

		using size_type = std::size_t;
		using value_type = T;
		using reference = value_type&;
		using const_reference = const value_type&;
		using rvalue_reference = value_type&&;
		using pointer = value_type*;
		using const_pointer = const value_type*;
		using EqualityFunctor = std::function<bool(const_reference lhs, const_reference rhs)>;
		using Allocator = std::pmr::polymorphic_allocator<T>;

	private:
		/// <summary>
		/// Singly linked list node
		/// </summary>
		struct Node final
		{

			template <typename... Args>
			Node(Node* next, Args&&... args);

			/// <summary>
			/// Creates a node from data and optionally the next node in the linked list
			/// </summary>
			/// <param name="data">Value to be contained within the node</param>
			/// <param name="next">Next node in the linked list</param>
			Node(const_reference data, Node* next = nullptr);
			Node(rvalue_reference data, Node* next = nullptr);
			Node* Next = nullptr;
			value_type Data;
		};

	public:
		class iterator;

		/// <summary>
		/// Iterator that when dereferenced returns a const version of a value in the container
		/// </summary>
		class const_iterator final
		{
			friend class SList;

		public:
			using iterator_category = std::forward_iterator_tag;
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
			const_iterator(const SList& container, Node* node = nullptr);

			const Node* _node = nullptr;
			const SList* _container = nullptr;
		};

		/// <summary>
		/// Iterator that when dereferenced returns a non-const version of a value in the container
		/// </summary>
		class iterator final
		{
			friend SList;
			friend const_iterator;

		public:
			using iterator_category = std::forward_iterator_tag;
			using difference_type = std::ptrdiff_t;
			using value_type = value_type;
			using pointer = value_type*;
			using reference = value_type&;

			iterator() = default;
			iterator& operator=(const iterator&) = default;

			/// <summary>
			/// Gets a const version of the value in the container pointed to by this iterator
			/// </summary>
			/// <exception cref="runtime_error">If the iterator is not associated with a container or points to an element not within a container</exception>
			/// <returns>value in the container</returns>
			[[nodiscard]] reference operator*() const;

			/// <summary>
			/// Gets a const version of the value in the container pointed to by this iterator
			/// </summary>
			/// <exception cref="runtime_error">If the iterator is not associated with a container or points to an element not within a container</exception>
			/// <returns>value in the container</returns>
			[[nodiscard]] pointer operator->() const;

			/// <summary>
			/// Increments the iterator so the iterator points to the next element in the container
			/// </summary>
			/// <exception cref="runtime_error">Iterator is not associated with a container</exception>
			/// <returns>Reference to the incremented iterator</returns>
			iterator& operator++();

			/// <summary>
			/// Increments the iterator so the iterator points to the next element in the container
			/// </summary>
			/// <param name=""></param>
			/// <exception cref="runtime_error">Iterator is not associated with a container</exception>
			/// <returns>Iterator before it is incremented</returns>
			iterator operator++(int);

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
			iterator(const SList& container, Node* node = nullptr);

			Node* _node = nullptr;
			const SList* _container = nullptr;
		};


		SList() = default;

		/// <summary>
		/// Creates a forward list from an initializer list of values
		/// </summary>
		/// <param name="list">initializer list of values</param>
		SList(std::initializer_list<value_type> list);

		/// <summary>
		/// Copy constructor
		/// </summary>
		SList(const SList& rhs);

		/// <summary>
		/// Move constructor
		/// </summary>
		SList(SList&& rhs) noexcept;

		/// <summary>
		/// Copy Assignment
		/// </summary>
		SList& operator=(const SList& rhs);

		/// <summary>
		/// Move Assignment
		/// </summary>
		SList& operator=(SList&& rhs) noexcept;

		~SList();

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
		/// Inserts a value after the given iterator.
		/// Providing an iterator pointing to the end of the container adds the value to the back of the container.
		/// </summary>
		/// <param name="it">Iterator to use as reference of where to insert the value</param>
		/// <param name="value">Value to insert</param>
		/// <exception cref="runtime_error">Iterator is not associated with a container</exception>
		void InsertAfter(const iterator& it, const_reference value);

		/// <summary>
		/// Finds the value in the container and return an iterator pointing to the value.
		/// If the value is not found, an iterator pointing to the end of the container is returned.
		/// </summary>
		/// <param name="value">Value to find</param>
		/// <param name="equalityFunctor">Functor to use for comparison</param>
		/// <returns>Iterator to the value or end of the container</returns>
		iterator Find(const_reference value, EqualityFunctor equalityFunctor = DefaultEquality<T>{});

		/// <summary>
		/// Finds the value in the container and return an iterator pointing to the value.
		/// If the value is not found, an iterator pointing to the end of the container is returned.
		/// </summary>
		/// <param name="value">Value to find</param>
		/// <param name="equalityFunctor">Functor to use for comparison</param>
		/// <returns>Const iterator to the value or end of the container</returns>
		const_iterator Find(const_reference value, EqualityFunctor equalityFunctor = DefaultEquality<T>{}) const;

		/// <summary>
		/// Removes the first instances of a value from the list.
		/// </summary>
		/// <param name="value">value to remove</param>
		/// <param name="equalityFunctor">Functor to use for comparison</param>
		/// <returns>True if the value was found in the list; otherwise false.</returns>
		bool Remove(const_reference value, EqualityFunctor equalityFunctor = DefaultEquality<T>{});

		/// <summary>
		/// Removes the value pointed to by the iterator from the list.
		/// If the iterator provided does not belong to this container or
		/// does not point to an element in the list, nothing is removed.
		/// </summary>
		/// <param name="it">iterator to use for removing</param>
		/// <returns>True if something was removed from the list; otherwise false.</returns>
		bool Remove(const iterator& it);

		/// <summary>
		/// Adds a value to the front of the list
		/// </summary>
		/// <param name="item">Item to add to the list</param>
		iterator PushFront(const_reference item);

		/// <summary>
		/// Adds a value to the front of the list
		/// </summary>
		/// <param name="item">Item to add to the list</param>
		iterator PushFront(rvalue_reference item);

		// TODO:
		template <typename... Args>
		iterator EmplaceFront(Args&&... args);

		/// <summary>
		/// Removes a value from the front of the list
		/// </summary>
		/// <exception cref="runtime_error">List is empty</exception>
		void PopFront();

		/// <summary>
		/// Adds a value to the back of the list
		/// </summary>
		/// <param name="item"></param>
		iterator PushBack(const_reference item);

		/// <summary>
		/// Adds a value to the back of the list
		/// </summary>
		/// <param name="item"></param>
		iterator PushBack(rvalue_reference item);

		// TODO:
		template <typename... Args>
		iterator EmplaceBack(Args&&... args);

		/// <summary>
		/// Removes a value from the back of the list
		/// </summary>
		/// <exception cref="runtime_error">List is empty</exception>
		void PopBack();

		/// <summary>
		/// Returns if the the list is empty
		/// </summary>
		/// <returns>True if the list is empty; otherwise false</returns>
		[[nodiscard]] bool IsEmpty() const;

		/// <summary>
		/// Gets the value at the front of the list
		/// </summary>
		/// <exception cref="runtime_error">List is empty</exception>
		/// <returns>Front value</returns>
		[[nodiscard]] reference Front();

		/// <summary>
		/// Gets the value at the front of the list
		/// </summary>
		/// <exception cref="runtime_error">List is empty</exception>
		/// <returns>Font value</returns>
		[[nodiscard]] const_reference Front() const;

		/// <summary>
		/// Gets the value at the back of the list
		/// </summary>
		/// <exception cref="runtime_error">List is empty</exception>
		/// <returns>Back value</returns>
		[[nodiscard]] reference Back();

		/// <summary>
		/// Gets the value at the back of the list
		/// </summary>
		/// <exception cref="runtime_error">List is empty</exception>
		/// <returns>Back value</returns>
		[[nodiscard]] const_reference Back() const;

		/// <summary>
		/// Gets the size of the list
		/// </summary>
		/// <returns>List size</returns>
		[[nodiscard]] std::size_t Size() const;

		/// <summary>
		/// Removes all values from the list
		/// </summary>
		void Clear();

	private:
		Allocator _alloc;
		std::size_t _size = 0;
		Node* _front = nullptr;
		Node* _back = nullptr;
	};
}

#include "SList.inl"