#pragma once

#include "Vector.h"
#include "SList.h"
#include "DefaultHash.h"

#include <functional>
#include <cstddef>
#include <utility>
#include <initializer_list>

namespace FieaGameEngine
{
	/// <summary>
	/// Unordered associative array
	/// </summary>
	/// <typeparam name="TKey">Key type</typeparam>
	/// <typeparam name="TData">Value type</typeparam>
	template <typename TKey, typename TData>
	class HashMap final
	{
	public:
		using PairType = std::pair<const TKey, TData>;
		using size_type = std::size_t;
		using value_type = PairType;
		using reference = value_type&;
		using const_reference = const value_type&;
		using rvalue_reference = value_type&&;
		using pointer = value_type*;
		using const_pointer = const value_type*;
		using Hash = std::function<std::size_t(const TKey&)>;
		using KeyEquality = std::function<bool(const TKey&, const TKey&)>;

	public:
		class iterator;

		/// <summary>
		/// Iterator that when dereferenced returns a const version of a value in the container
		/// </summary>
		class const_iterator final
		{
			friend class HashMap;

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
			/// Gets a const version of the value in the container pointed to by this iterator
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
			const_iterator(const HashMap& _container, Vector<SList<value_type>>::const_iterator _containerIterator, SList<value_type>::const_iterator _chainIterator);

			const HashMap* _container = nullptr;
			Vector<SList<value_type>>::const_iterator _containerIterator;
			SList<PairType>::const_iterator _chainIterator;
		};

		/// <summary>
		/// Iterator that when dereferenced returns a non-const version of a value in the container
		/// </summary>
		class iterator final
		{
			friend HashMap;
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
			iterator(HashMap& _container, Vector<SList<value_type>>::iterator _containerIterator, SList<value_type>::iterator _chainIterator);

			HashMap* _container = nullptr;
			Vector<SList<value_type>>::iterator _containerIterator;
			SList<PairType>::iterator _chainIterator;
		};

		explicit HashMap(size_t bucketCount = _DEFAULT_BUCKET_COUNT, Hash hash = DefaultHash<TKey>{}, KeyEquality keyEquality = DefaultEquality<TKey>{});
		HashMap(const HashMap& other) = default;
		HashMap(HashMap&& other) = default;
		HashMap(std::initializer_list<PairType> list);
		~HashMap() = default;

		HashMap& operator=(const HashMap& rhs) = default;
		HashMap& operator=(HashMap&& rhs) = default;

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
		[[nodiscard]] const_iterator cbegin();

		/// <summary>
		/// Gets a const_iterator pointing to one past the last element in the container
		/// </summary>
		/// <returns>const_iterator at the end of the container</returns>
		[[nodiscard]] const_iterator cend();

		/// <summary>
		/// Finds a element in the containers and returns an iterator to the element.
		/// If no element is found, an iterator pointing to the end of the container is returned.
		/// </summary>
		/// <param name="value">Value to find</param>
		/// <returns>Iterator pointing to the value found or to the end of the container</returns>
		[[nodiscard]] iterator Find(const TKey& key);

		/// <summary>
		/// Finds a element in the containers and returns an iterator to the element.
		/// If no element is found, an iterator pointing to the end of the container is returned.
		/// </summary>
		/// <param name="value">Value to find</param>
		/// <returns>Iterator pointing to the value found or to the end of the container</returns>
		[[nodiscard]] const_iterator Find(const TKey& key) const;

		/// <summary>
		/// Adds a value at the key provided container. If the key already exists,
		/// and iterator to the already existing key value pair is returned.
		/// </summary>
		/// <param name="keyValuePair">Key value pair to add</param>
		/// <returns>Iterator pointing to the key value pair. Boolean indicates if the value was inserted</returns>
		std::pair<iterator, bool> Insert(const PairType& keyValuePair);
		
		/// <summary>
		/// Adds a value at the key provided container. If the key already exists,
		/// and iterator to the already existing key value pair is returned.
		/// </summary>
		/// <param name="keyValuePair">Key value pair to add</param>
		/// <returns>Iterator pointing to the key value pair. Boolean indicates if the value was inserted</returns>
		std::pair<iterator, bool> Insert(PairType&& keyValuePair);
		
		/// <summary>
		/// Adds a value at the key provided container. If the key already exists,
		/// and iterator to the already existing key value pair is returned.
		/// </summary>
		/// <param name="keyValuePair">Key value pair to add</param>
		/// <returns>Iterator pointing to the key value pair. Boolean indicates if the value was inserted</returns>
		template <typename... Args>
		std::pair<iterator, bool> Emplace(Args&&... args);
		
		/// <summary>
		/// Gets the value at the specified key. If the key does not exist in the container,
		/// a value is default constructed and inserted into the container at the key's location
		/// </summary>
		/// <param name="key">Key to get</param>
		/// <returns>Data associated with the key in the container</returns>
		TData& operator[](const TKey& key);

		/// <summary>
		/// Removes the key and value from the container given a key
		/// </summary>
		/// <param name="key">Key to remove</param>
		/// <returns>Whether the key was found</returns>
		bool Remove(const TKey& key);

		// TODO: remove that takes in iterator

		/// <summary>
		/// Clears the containers of all keys and values
		/// </summary>
		void Clear();

		/// <summary>
		/// Gets the number of elements in the container
		/// </summary>
		/// <returns>Number of elements in the container</returns>
		[[nodiscard]] std::size_t Size() const;

		/// <summary>
		/// Gets the number of buckets in the container
		/// </summary>
		/// <returns>Number of buckets in the container</returns>
		[[nodiscard]] std::size_t BucketCount() const;

		/// <summary>
		/// The percentage of buckets in use in the container from [0, 1].
		/// 0 means the container is empty, and 1 means the container is full.
		/// When the load factor increases, the container becomes more susceptible to collisions on insert 
		/// </summary>
		/// <returns>Percentage of buckets in use from 0 to 1</returns>
		[[nodiscard]] float LoadFactor() const;

		/// <summary>
		/// Checks if a key is in the container
		/// </summary>
		/// <param name="key"></param>
		/// <returns>True if the key exists; otherwise false</returns>
		[[nodiscard]] bool ContainsKey(const TKey& key) const;
		
		/// <summary>
		/// Gets the value at the key.
		/// </summary>
		/// <param name="key"></param>
		/// <exception cref="runtime_error">Key does not exist in the container</exception>
		/// <returns>Data at the given key in the container</returns>
		[[nodiscard]] TData& At(const TKey& key);

		/// <summary>
		/// Gets the value at the key.
		/// </summary>
		/// <param name="key"></param>
		/// <exception cref="runtime_error">Key does not exist in the container</exception>
		/// <returns>Data at the given key in the container</returns>
		[[nodiscard]] const TData& At(const TKey& key) const;

		/// <summary>
		/// Resizes the number of buckets in the container. Causes a rehash of all key value pairs
		/// currently in the container
		/// </summary>
		/// <exception cref="runtime_error">New bucket count must be greater than current bucket count</exception>
		/// <param name="bucketCount">New bucket count</param>
		void Resize(size_t bucketCount);

	private:
		KeyEquality _keyEquality;
		Hash _hash;
		Vector<SList<PairType>> _buckets;
		std::size_t _numElements = 0;
		std::size_t _bucketsInUse = 0;
		static const std::size_t _DEFAULT_BUCKET_COUNT = 11;

		// Helper methods for Find() that output index to prevent duplicate hashing
		[[nodiscard]] iterator _Find(const TKey& key, size_t& index);
		[[nodiscard]] const_iterator _Find(const TKey& key, size_t& index) const;
	};
}

#include "HashMap.inl"
