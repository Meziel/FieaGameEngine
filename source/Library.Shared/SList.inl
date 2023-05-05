#include "SList.h"
#include <stdexcept>
#include <utility>

namespace FieaGameEngine
{
#pragma region Node
	template<typename T>
	template<typename ...Args>
	inline SList<T>::Node::Node(Node* next, Args&&... args) :
		Data(std::forward<Args>(args)...), Next(next)
	{
	}

	template <typename T>
	inline SList<T>::Node::Node(const_reference data, Node* next) :
		Data(data), Next(next)
	{
	}

	template<typename T>
	inline SList<T>::Node::Node(rvalue_reference data, Node* next) :
		Data(std::move(data)), Next(next)
	{
	}
#pragma endregion Node

#pragma region SList
	template <typename T>
	inline SList<T>::SList(std::initializer_list<value_type> list)
	{
		for (const auto& value : list)
		{
			PushBack(value);
		}
	}

	template <typename T>
	SList<T>::SList(const SList& rhs)
	{
		Node* currentNode = rhs._front;
		while (currentNode != nullptr)
		{
			PushBack(currentNode->Data);
			currentNode = currentNode->Next;
		}
	}

	template<typename T>
	inline SList<T>::SList(SList&& rhs) noexcept :
		_front(rhs._front), _back(rhs._back), _size(rhs._size)
	{
		rhs._front = nullptr;
		rhs._back = nullptr;
		rhs._size = 0;
	}

	template <typename T>
	SList<T>& SList<T>::operator=(const SList& rhs)
	{
		if (this != &rhs)
		{
			// Clear list
			Clear();

			// Copy from rhs to lhs
			Node* currentNode = rhs._front;
			while (currentNode != nullptr)
			{
				PushBack(currentNode->Data);
				currentNode = currentNode->Next;
			}
		}

		return *this;
	}

	template <typename T>
	SList<T>& SList<T>::operator=(SList&& rhs) noexcept
	{
		if (this != &rhs)
		{
			// Clear list
			Clear();

			_front = rhs._front;
			_back = rhs._back;
			_size = rhs._size;

			rhs._front = nullptr;
			rhs._back = nullptr;
			rhs._size = 0;
		}

		return *this;
	}

	template <typename T>
	SList<T>::~SList()
	{
		Clear();
	}

	template <typename T>
	inline SList<T>::iterator SList<T>::PushFront(const_reference item)
	{
		return EmplaceFront(item);
	}

	template<typename T>
	inline SList<T>::iterator SList<T>::PushFront(rvalue_reference item)
	{
		return EmplaceFront(std::move(item));
	}

	template<typename T>
	template <typename... Args>
	inline SList<T>::iterator SList<T>::EmplaceFront(Args&&... args)
	{
		// Add new front
		_front = _alloc.new_object<Node>(_front, std::forward<Args>(args)...);
		++_size;
		if (_size == 1)
		{
			_back = _front;
		}
		return iterator(*this, _front);
	}

	template <typename T>
	inline void SList<T>::PopFront()
	{
		if (_size == 0)
		{
			throw std::runtime_error("List is empty");
		}

		Node* frontToDelete = _front;
		// Set front to front + 1
		_front = _front->Next;
		// Delete front
		_alloc.delete_object<Node>(frontToDelete);
		--_size;
		if (_size == 0)
		{
			_back = nullptr;
		}
	}

	template <typename T>
	inline SList<T>::iterator SList<T>::PushBack(const_reference item)
	{
		return EmplaceBack(item);
	}

	template<typename T>
	inline SList<T>::iterator SList<T>::PushBack(rvalue_reference item)
	{
		return EmplaceBack(std::move(item));
	}

	template<typename T>
	template<typename... Args>
	inline SList<T>::iterator SList<T>::EmplaceBack(Args&&... args)
	{
		Node* previousBack = _back;
		// Create new back
		_back = _alloc.new_object<Node>(nullptr, std::forward<Args>(args)...);

		// Link previous back next 
		if (previousBack != nullptr)
		{
			previousBack->Next = _back;
		}
		++_size;
		if (_size == 1)
		{
			_front = _back;
		}
		return iterator(*this, _back);
	}

	template <typename T>
	inline void SList<T>::PopBack()
	{
		if (_size == 0)
		{
			throw std::runtime_error("List is empty");
		}

		// Update back to back - 1
		Node* currentNode = _front;
		// Trivial case where list is size 1
		if (_size == 1)
		{
			_alloc.delete_object<Node>(_back);
			_front = nullptr;
			_back = nullptr;
		}
		// Loop until back - 1 and update next to nullptr
		else
		{
			while (currentNode->Next != _back)
			{
				currentNode = currentNode->Next;
			}
			currentNode->Next = nullptr;
			_alloc.delete_object<Node>(_back);
			_back = currentNode;
		}
		--_size;
	}

	template <typename T>
	inline bool SList<T>::IsEmpty() const
	{
		return _size == 0;
	}

	template <typename T>
	inline T& SList<T>::Front()
	{
		if (_size == 0)
		{
			throw std::runtime_error("List is empty");
		}
		return _front->Data;
	}

	template <typename T>
	inline const T& SList<T>::Front() const
	{
		if (_size == 0)
		{
			throw std::runtime_error("List is empty");
		}
		return _front->Data;
	}

	template <typename T>
	inline T& SList<T>::Back()
	{
		if (_size == 0)
		{
			throw std::runtime_error("List is empty");
		}
		return _back->Data;
	}

	template <typename T>
	inline const T& SList<T>::Back() const
	{
		if (_size == 0)
		{
			throw std::runtime_error("List is empty");
		}
		return _back->Data;
	}

	template <typename T>
	inline std::size_t SList<T>::Size() const
	{
		return _size;
	}

	template <typename T>
	inline void SList<T>::Clear()
	{
		while (_size > 0)
		{
			PopFront();
		}
	}

	template <typename T>
	typename SList<T>::iterator SList<T>::begin()
	{
		return iterator(*this, _front);
	}

	template <typename T>
	typename SList<T>::const_iterator SList<T>::begin() const
	{
		return const_iterator(*this, _front);
	}

	template <typename T>
	typename SList<T>::iterator SList<T>::end()
	{
		return iterator(*this);
	}

	template <typename T>
	typename SList<T>::const_iterator SList<T>::end() const
	{
		return const_iterator(*this);
	}

	template <typename T>
	typename SList<T>::const_iterator SList<T>::cbegin() const
	{
		return const_iterator(*this, _front);
	}

	template <typename T>
	typename SList<T>::const_iterator SList<T>::cend() const
	{
		return const_iterator(*this);
	}

	template <typename T>
	void SList<T>::InsertAfter(const iterator& it, const_reference value)
	{
		if (it._container != this)
		{
			throw std::runtime_error("Iterator is not associated with a container");
		}

		if (it._node == nullptr)
		{
			PushBack(value);
		}
		else
		{
			it._node->Next = _alloc.new_object<Node>(value, it._node->Next);
			++_size;
		}
	}

	template <typename T>
	typename SList<T>::iterator SList<T>::Find(const_reference value, EqualityFunctor equalityFunctor)
	{
		iterator it = begin();
		while (it != end())
		{
			if (equalityFunctor(*it, value))
			{
				break;
			}
			++it;
		}
		return it;
	}

	template <typename T>
	typename SList<T>::const_iterator SList<T>::Find(const_reference value, EqualityFunctor equalityFunctor) const
	{
		const_iterator it = begin();
		while (it != end())
		{
			if (equalityFunctor(*it, value))
			{
				break;
			}
			++it;
		}
		return it;
	}

	template <typename T>
	bool SList<T>::Remove(const_reference value, EqualityFunctor equalityFunctor)
	{
		return Remove(Find(value, equalityFunctor));
	}

	template <typename T>
	bool SList<T>::Remove(const iterator& it)
	{
		if (it._container != this)
		{
			return false;
		}

		if (it == end())
		{
			return false;
		}

		if (it._node == _back)
		{
			PopBack();
		}
		else
		{
			Node* next = it._node->Next;
			it._node->Data.~T();
			_alloc.construct(&it._node->Data, std::move(next->Data));
			it._node->Next = next->Next;
			_alloc.delete_object<Node>(next);

			--_size;
		}

		return true;
	}
#pragma endregion SList

#pragma region const_iterator
	template <typename T>
	SList<T>::const_iterator::const_iterator(const iterator& other) :
		_container(other._container), _node(other._node)
	{
	}

	template <typename T>
	SList<T>::const_iterator::const_iterator(const SList& container, Node* node) :
		_container(&container),
		_node(node)
	{
	}

	template <typename T>
	inline SList<T>::const_reference SList<T>::const_iterator::operator*() const
	{
		if (_node == nullptr)
		{
			throw std::runtime_error("Iterator does not point to an element in the container");
		}
		return _node->Data;
	}

	template <typename T>
	inline const typename SList<T>::const_iterator::value_type* SList<T>::const_iterator::operator->() const
	{
		return &operator*();
	}

	template <typename T>
	inline typename SList<T>::const_iterator& SList<T>::const_iterator::operator++()
	{
		if (_container == nullptr)
		{
			throw std::runtime_error("Iterator is not associated with a container");
		}

		if (_node != nullptr)
		{
			_node = _node->Next;
		}
		return *this;
	}

	template <typename T>
	inline typename SList<T>::const_iterator SList<T>::const_iterator::operator++(int)
	{
		const_iterator it = *this;
		operator++();
		return it;
	}

	template <typename T>
	bool SList<T>::const_iterator::operator==(const const_iterator& other) const
	{
		return !operator!=(other);
	}

	template <typename T>
	inline bool SList<T>::const_iterator::operator!=(const const_iterator& other) const
	{
		if (_container != other._container)
		{
			return true;
		}

		return _node != other._node;
	}
#pragma endregion const_iterator

#pragma region iterator
	template <typename T>
	SList<T>::iterator::iterator(const SList& container, Node* node) :
		_container(&container),
		_node(node)
	{
	}

	template <typename T>
	inline SList<T>::reference SList<T>::iterator::operator*() const
	{
		if (_node == nullptr)
		{
			throw std::runtime_error("Iterator does not point to an element in the container");
		}
		return _node->Data;
	}

	template <typename T>
	inline typename SList<T>::pointer SList<T>::iterator::operator->() const
	{
		return &operator*();
	}

	template <typename T>
	inline typename SList<T>::iterator& SList<T>::iterator::operator++()
	{
		if (_container == nullptr)
		{
			throw std::runtime_error("Iterator is not associated with a container");
		}

		if (_node != nullptr)
		{
			_node = _node->Next;
		}
		return *this;
	}

	template <typename T>
	inline typename SList<T>::iterator SList<T>::iterator::operator++(int)
	{
		iterator it = *this;
		operator++();
		return it;
	}

	template <typename T>
	inline bool SList<T>::iterator::operator==(const iterator& other) const
	{
		return !operator!=(other);
	}

	template <typename T>
	inline bool SList<T>::iterator::operator!=(const iterator& other) const
	{
		if (_container != other._container)
		{
			return true;
		}

		return _node != other._node;
	}
#pragma endregion iterator
}
