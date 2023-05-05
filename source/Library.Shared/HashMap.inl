#include "HashMap.h"

namespace FieaGameEngine
{
#pragma region HashMap
	template<typename TKey, typename TData>
	inline FieaGameEngine::HashMap<TKey, TData>::HashMap(std::size_t bucketCount, Hash hash, KeyEquality keyEquality) :
		_buckets(bucketCount),
		_hash(hash),
		_keyEquality(keyEquality)
	{
		assert(bucketCount > 0);
		_buckets.Resize(bucketCount);
	}

	template<typename TKey, typename TData>
	inline HashMap<TKey, TData>::HashMap(std::initializer_list<PairType> list) : HashMap(list.size())
	{
		for (const PairType& pair : list)
		{
			Insert(pair);
		}
	}

	template<typename TKey, typename TData>
	inline typename HashMap<TKey, TData>::iterator HashMap<TKey, TData>::Find(const TKey& key)
	{
		std::size_t index;
		return _Find(key, index);
	}

	template<typename TKey, typename TData>
	inline typename HashMap<TKey, TData>::const_iterator HashMap<TKey, TData>::Find(const TKey& key) const
	{
		std::size_t index;
		return _Find(key, index);
	}

	template<typename TKey, typename TData>
	inline std::pair<typename HashMap<TKey, TData>::iterator, bool> HashMap<TKey, TData>::Insert(const PairType& keyValuePair)
	{
		return Emplace(keyValuePair);
	}

	template<typename TKey, typename TData>
	inline std::pair<typename HashMap<TKey, TData>::iterator, bool> HashMap<TKey, TData>::Insert(PairType&& keyValuePair)
	{
		return Emplace(std::move(keyValuePair));
	}

	template<typename TKey, typename TData>
	template <typename... Args>
	inline std::pair<typename HashMap<TKey, TData>::iterator, bool> HashMap<TKey, TData>::Emplace(Args&&... args)
	{
		// Construct key
		PairType keyValuePair(std::forward<Args>(args)...);
		size_t index;
		iterator it = _Find(keyValuePair.first, index);
		if (it != end())
		{
			// Key already exists in the hashmap
			return std::pair(it, false);
		}
		// Key does not already exist in the hashmap
		SList<PairType>& chain = _buckets[index];
		if (chain.Size() == 0)
		{
			// First element in this chain. Increase counter for Load Capacity calculation
			++_bucketsInUse;
		}
		// Size
		++_numElements;
		it = iterator(*this, _buckets.begin() + index, chain.PushBack(std::move(keyValuePair)));
		return std::pair(it, true);
	}

	template<typename TKey, typename TData>
	inline TData& HashMap<TKey, TData>::operator[](const TKey& key)
	{
		iterator it = Insert({ key, TData{} }).first;
		return it->second;
	}

	template<typename TKey, typename TData>
	inline bool HashMap<TKey, TData>::Remove(const TKey& key)
	{
		iterator it = Find(key);
		if (it == end())
		{
			// Key not found
			return false;
		}
		// Key found
		SList<PairType>& chain = *it._containerIterator;
		chain.Remove(it._chainIterator);
		if (chain.Size() == 0)
		{
			--_bucketsInUse;
		}
		--_numElements;
		return true;
	}

	template<typename TKey, typename TData>
	inline void HashMap<TKey, TData>::Clear()
	{
		// Clear the chains without removing them
		for (size_t i = 0; i < _buckets.Size(); ++i)
		{
			_buckets[i].Clear();
		}
		_bucketsInUse = 0;
		_numElements = 0;
	}

	template<typename TKey, typename TData>
	inline std::size_t HashMap<TKey, TData>::Size() const
	{
		return _numElements;
	}

	template<typename TKey, typename TData>
	inline std::size_t HashMap<TKey, TData>::BucketCount() const
	{
		return _buckets.Size();
	}

	template<typename TKey, typename TData>
	inline float HashMap<TKey, TData>::LoadFactor() const
	{
		return static_cast<float>(_bucketsInUse) / _buckets.Size();
	}

	template<typename TKey, typename TData>
	inline bool HashMap<TKey, TData>::ContainsKey(const TKey& key) const
	{
		return Find(key) != end();
	}

	template<typename TKey, typename TData>
	inline TData& HashMap<TKey, TData>::At(const TKey& key)
	{
		iterator it = Find(key);
		if (it == end())
		{
			throw std::runtime_error("Key does not exist in the container");
		}
		return it->second;
	}

	template<typename TKey, typename TData>
	inline const TData& HashMap<TKey, TData>::At(const TKey& key) const
	{
		const_iterator it = Find(key);
		if (it == end())
		{
			throw std::runtime_error("Key does not exist in the container");
		}
		return it->second;
	}

	template<typename TKey, typename TData>
	inline void HashMap<TKey, TData>::Resize(size_t bucketCount)
	{
		if (bucketCount < _buckets.Size())
		{
			throw std::runtime_error("New bucket count must be greater than current bucket count");
		}
		Vector<SList<PairType>> newBuckets(bucketCount);
		newBuckets.Resize(bucketCount);
		for (auto& pair : *this)
		{
			newBuckets[_hash(pair.first) % bucketCount].PushBack(std::move(pair));
		}
		_buckets = std::move(newBuckets);
	}

	template<typename TKey, typename TData>
	inline typename HashMap<TKey, TData>::iterator HashMap<TKey, TData>::begin()
	{
		typename Vector<SList<PairType>>::iterator beg = _buckets.begin();
		SList<PairType>& firstChain = *beg;
		iterator it = iterator(*this, beg, firstChain.begin());
		if (firstChain.Size() == 0)
		{
			// First bucket is empty. Iterate until node found or end
			++it;
		}
		return it;
	}

	template<typename TKey, typename TData>
	inline typename HashMap<TKey, TData>::const_iterator HashMap<TKey, TData>::begin() const
	{
		const typename Vector<SList<PairType>>::const_iterator beg = _buckets.begin();
		const SList<PairType>& firstChain = *beg;
		const_iterator it(*this, beg, firstChain.begin());
		if (firstChain.Size() == 0)
		{
			// First bucket is empty. Iterate until node found or end
			++it;
		}
		return it;
	}

	template<typename TKey, typename TData>
	inline typename HashMap<TKey, TData>::iterator HashMap<TKey, TData>::end()
	{
		return iterator(*this, _buckets.end(), typename SList<PairType>::iterator{});
	}

	template<typename TKey, typename TData>
	inline typename HashMap<TKey, TData>::const_iterator HashMap<TKey, TData>::end() const
	{
		return const_iterator(*this, _buckets.end(), typename SList<PairType>::const_iterator{});
	}

	template<typename TKey, typename TData>
	inline typename HashMap<TKey, TData>::const_iterator HashMap<TKey, TData>::cbegin()
	{
		const typename Vector<SList<PairType>>::const_iterator beg = _buckets.begin();
		const SList<PairType>& firstChain = *beg;
		const_iterator it(*this, beg, firstChain.begin());
		if (firstChain.Size() == 0)
		{
			// First bucket is empty. Iterate until node found or end
			++it;
		}
		return it;
	}

	template<typename TKey, typename TData>
	inline typename HashMap<TKey, TData>::const_iterator HashMap<TKey, TData>::cend()
	{
		return const_iterator(*this, _buckets.end(), typename SList<PairType>::const_iterator{});
	}

	template<typename TKey, typename TData>
	inline typename HashMap<TKey, TData>::iterator HashMap<TKey, TData>::_Find(const TKey& key, size_t& index)
	{
		if (_buckets.size() == 0)
		{
			return end();
		}
		index = _hash(key) % _buckets.Size();
		SList<PairType>& chain = _buckets[index];
		typename SList<PairType>::iterator it = chain.Find(
			std::pair(key, TData{}),
			[this](const PairType& lhs, const PairType& rhs) { return this->_keyEquality(lhs.first, rhs.first); });
		if (it == chain.end())
		{
			// Key does not exist
			return end();
		}
		// Key found
		return iterator(*this, _buckets.begin() + index, it);
	}

	template<typename TKey, typename TData>
	inline typename HashMap<TKey, TData>::const_iterator HashMap<TKey, TData>::_Find(const TKey& key, size_t& index) const
	{
		index = _hash(key) % _buckets.Size();
		const SList<PairType>& chain = _buckets[index];
		typename SList<PairType>::const_iterator it = chain.Find(
			std::pair(key, TData{}),
			[this](const PairType& lhs, const PairType& rhs) { return this->_keyEquality(lhs.first, rhs.first); });
		if (it == chain.end())
		{
			// Key does not exist
			return end();
		}
		return const_iterator(*this, _buckets.begin() + index, it);
	}
#pragma endregion HashMap

#pragma region const_iterator
	template<typename TKey, typename TData>
	inline HashMap<TKey, TData>::const_iterator::const_iterator(const iterator& other) :
		_container(other._container),
		_containerIterator(other._containerIterator),
		_chainIterator(other._chainIterator)
	{
	}

	template <typename TKey, typename TData>
	inline HashMap<TKey, TData>::const_iterator::const_iterator(const HashMap& _container, Vector<SList<value_type>>::const_iterator _containerIterator, SList<value_type>::const_iterator _chainIterator) :
		_container(&_container),
		_containerIterator(_containerIterator),
		_chainIterator(_chainIterator)
	{
	}

	template<typename TKey, typename TData>
	inline typename HashMap<TKey, TData>::const_reference HashMap<TKey, TData>::const_iterator::operator*() const
	{
		if (_container == nullptr)
		{
			throw std::runtime_error("Iterator is not associated with a container");
		}

		if (_containerIterator == _container->_buckets.end() || _chainIterator == _containerIterator->end())
		{
			throw std::runtime_error("Iterator does not point to an element in the container");
		}

		return *_chainIterator;

	}

	template<typename TKey, typename TData>
	inline const typename HashMap<TKey, TData>::const_iterator::value_type* HashMap<TKey, TData>::const_iterator::operator->() const
	{
		return &operator*();
	}

	template<typename TKey, typename TData>
	inline typename HashMap<TKey, TData>::const_iterator& HashMap<TKey, TData>::const_iterator::operator++()
	{
		if (_container == nullptr)
		{
			throw std::runtime_error("Iterator is not associated with a container");
		}

		auto end = _container->_buckets.end();

		// If at vector end do nothing
		if (_containerIterator == end)
		{
			return *this;
		}

		// Go until vector end or slist node is found
		while (true)
		{
			// Go to next node
			if (_chainIterator != _containerIterator->end())
			{
				++_chainIterator;
			}
			else
			{
				++_containerIterator;
				if (_containerIterator == end)
				{
					// Hit end of vector
					_chainIterator = typename SList<value_type>::iterator{};
					break;
				}
				_chainIterator = _containerIterator->begin();
			}

			// Stop when node is found
			if (_chainIterator != _containerIterator->end())
			{
				break;
			}
		}

		return *this;
	}

	template<typename TKey, typename TData>
	inline typename HashMap<TKey, TData>::const_iterator HashMap<TKey, TData>::const_iterator::operator++(int)
	{
		const_iterator it = *this;
		operator++();
		return it;
	}

	template<typename TKey, typename TData>
	inline bool HashMap<TKey, TData>::const_iterator::operator==(const const_iterator& other) const
	{
		return !operator!=(other);
	}

	template<typename TKey, typename TData>
	bool HashMap<TKey, TData>::const_iterator::operator!=(const const_iterator& other) const
	{
		if (_container != other._container)
		{
			return true;
		}

		if (_container == nullptr)
		{
			return false;
		}

		auto end = _container->_buckets.end();

		if (_containerIterator == end && other._containerIterator == end)
		{
			return false;
		}

		return _containerIterator != other._containerIterator || _chainIterator != other._chainIterator;
	}
#pragma endregion const_iterator

#pragma region iterator
	template <typename TKey, typename TData>
	inline HashMap<TKey, TData>::iterator::iterator(HashMap& _container, Vector<SList<value_type>>::iterator _containerIterator, SList<value_type>::iterator _chainIterator) :
		_container(&_container),
		_containerIterator(_containerIterator),
		_chainIterator(_chainIterator)
	{
	}

	template<typename TKey, typename TData>
	inline typename HashMap<TKey, TData>::reference HashMap<TKey, TData>::iterator::operator*() const
	{
		if (_container == nullptr)
		{
			throw std::runtime_error("Iterator is not associated with a container");
		}

		if (_containerIterator == _container->_buckets.end() || _chainIterator == _containerIterator->end())
		{
			throw std::runtime_error("Iterator does not point to an element in the container");
		}

		return *_chainIterator;

	}

	template<typename TKey, typename TData>
	inline typename HashMap<TKey, TData>::pointer HashMap<TKey, TData>::iterator::operator->() const
	{
		return &operator*();
	}

	template<typename TKey, typename TData>
	inline typename HashMap<TKey, TData>::iterator& HashMap<TKey, TData>::iterator::operator++()
	{
		if (_container == nullptr)
		{
			throw std::runtime_error("Iterator is not associated with a container");
		}

		auto end = _container->_buckets.end();

		// If at vector end do nothing
		if (_containerIterator == end)
		{
			return *this;
		}

		// Go until vector end or slist node is found
		while (true)
		{
			// Go to next node
			if (_chainIterator != _containerIterator->end())
			{
				++_chainIterator;
			}
			else
			{
				++_containerIterator;
				if (_containerIterator == end)
				{
					// Hit end of vector
					_chainIterator = typename SList<value_type>::iterator{};
					break;
				}
				_chainIterator = _containerIterator->begin();
			}
			
			// Stop when node is found
			if (_chainIterator != _containerIterator->end())
			{
				break;
			}
		}

		return *this;
	}

	template<typename TKey, typename TData>
	inline typename HashMap<TKey, TData>::iterator HashMap<TKey, TData>::iterator::operator++(int)
	{
		iterator it = *this;
		operator++();
		return it;
	}

	template<typename TKey, typename TData>
	inline bool HashMap<TKey, TData>::iterator::operator==(const iterator& other) const
	{
		return !operator!=(other);
	}

	template<typename TKey, typename TData>
	bool HashMap<TKey, TData>::iterator::operator!=(const iterator& other) const
	{
		if (_container != other._container)
		{
			return true;
		}

		if (_container == nullptr)
		{
			return false;
		}

		auto end = _container->_buckets.end();

		if (_containerIterator == end && other._containerIterator == end)
		{
			return false;
		}

		return _containerIterator != other._containerIterator || _chainIterator != other._chainIterator;
	}
#pragma endregion iterator
}