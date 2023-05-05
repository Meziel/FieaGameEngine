#include "Vector.h"
#include <utility>
#include <cassert>
#include <algorithm>

namespace FieaGameEngine
{
#pragma region Vector
	template<typename T>
	inline Vector<T>::Vector(std::size_t capacity, ReserveStrategy reserveStrategy) :
		_reserveStrategy(reserveStrategy)
	{
		Reserve(capacity);
	}

	template<typename T>
	inline Vector<T>::Vector(std::initializer_list<value_type> list)
	{
		Reserve(list.size());
		for (const auto& value : list)
		{
			PushBack(value);
		}
	}

	template<typename T>
	inline Vector<T>::Vector(const Vector& other)
	{
		Reserve(other._size);
		for (size_t i = 0; i < other._size; ++i)
		{
			PushBack(other[i]);
		}
	}

	template<typename T>
	inline Vector<T>::Vector(Vector&& other) noexcept :
		_data(other._data), _size(other._size), _capacity(other._capacity)
	{
		other._data = nullptr;
		other._size = 0;
		other._capacity = 0;
	}

	template<typename T>
	inline Vector<T>& Vector<T>::operator=(const Vector& rhs)
	{
		if (this != &rhs)
		{
			Clear();
			Reserve(rhs._size);
			for (size_t i = 0; i < rhs._size; ++i)
			{
				PushBack(rhs[i]);
			}
		}
		return *this;
	}

	template<typename T>
	inline Vector<T>& Vector<T>::operator=(Vector&& rhs) noexcept
	{
		if (this != &rhs)
		{
			Clear();
			ShrinkToFit();
			_data = rhs._data;
			_size = rhs._size;
			_capacity = rhs._capacity;

			rhs._data = nullptr;
			rhs._size = 0;
			rhs._capacity = 0;
		}
		return *this;
	}

	template<typename T>
	inline Vector<T>::~Vector()
	{
		Clear();
		ShrinkToFit();
	}

	template<typename T>
	inline Vector<T>::iterator Vector<T>::begin()
	{
		return iterator(*this, _data);
	}

	template<typename T>
	inline Vector<T>::const_iterator Vector<T>::begin() const
	{
		return const_iterator(*this, _data);
	}

	template<typename T>
	inline Vector<T>::iterator Vector<T>::end()
	{
		return iterator(*this, _data + _size);
	}

	template<typename T>
	inline Vector<T>::const_iterator Vector<T>::end() const
	{
		return const_iterator(*this, _data + _size);
	}

	template<typename T>
	inline Vector<T>::const_iterator Vector<T>::cbegin() const
	{
		return const_iterator(*this, _data);
	}

	template<typename T>
	inline Vector<T>::const_iterator Vector<T>::cend() const
	{
		return const_iterator(*this, _data + _size);
	}

	template<typename T>
	inline Vector<T>::reference Vector<T>::operator[](std::size_t index)
	{
		return At(index);
	}

	template<typename T>
	inline Vector<T>::const_reference Vector<T>::operator[](std::size_t index) const
	{
		return At(index);
	}

	template<typename T>
	inline Vector<T>::reference Vector<T>::At(std::size_t index)
	{
		if (index >= _size)
		{
			throw std::runtime_error("Index out of bounds");
		}
		return _data[index];
	}

	template<typename T>
	inline Vector<T>::const_reference Vector<T>::At(std::size_t index) const
	{
		if (index >= _size)
		{
			throw std::runtime_error("Index out of bounds");
		}
		return _data[index];
	}

	template<typename T>
	inline void Vector<T>::PopBack()
	{
		if (_size == 0)
		{
			throw std::runtime_error("Vector is empty");
		}
		_data[_size - 1].~T();
		--_size;
	}

	template<typename T>
	inline bool Vector<T>::IsEmpty() const
	{
		return _size == 0;
	}

	template<typename T>
	inline Vector<T>::reference Vector<T>::Front()
	{
		if (_size == 0)
		{
			throw std::runtime_error("Vector is empty");
		}
		return At(0);
	}

	template<typename T>
	inline Vector<T>::const_reference Vector<T>::Front() const
	{
		if (_size == 0)
		{
			throw std::runtime_error("Vector is empty");
		}
		return At(0);
	}

	template<typename T>
	inline Vector<T>::reference Vector<T>::Back()
	{
		if (_size == 0)
		{
			throw std::runtime_error("Vector is empty");
		}
		return At(_size - 1);
	}

	template<typename T>
	inline Vector<T>::const_reference Vector<T>::Back() const
	{
		if (_size == 0)
		{
			throw std::runtime_error("Vector is empty");
		}
		return At(_size - 1);
	}

	template<typename T>
	inline std::size_t Vector<T>::Size() const
	{
		return _size;
	}

	template<typename T>
	inline std::size_t Vector<T>::Capacity() const
	{
		return _capacity;
	}

	template <typename T>
	void Vector<T>::Reserve(size_t capacity)
	{
		if (capacity > _capacity)
		{
			pointer data = reinterpret_cast<pointer>(realloc(_data, capacity * sizeof(value_type)));
			assert(data != nullptr);
			_data = data;
			_capacity = capacity;
		}
	}

	template<typename T>
	inline void Vector<T>::Resize(size_t size)
	{
		if (size == _size)
		{
			return;
		}

		if (size < _size)
		{
			Remove(begin() + size, end());
		}
		else
		{
			size_t diff = size - _size;
			Reserve(size);
			for (size_t i = 0; i < diff; ++i)
			{
				PushBack(T{});
			}
		}
	}

	template<typename T>
	inline void Vector<T>::Resize(size_t size, const_reference value)
	{
		if (size == _size)
		{
			return;
		}

		if (size < _size)
		{
			Remove(begin() + size, end());
		}
		else
		{
			size_t diff = size - _size;
			Reserve(size);
			for (size_t i = 0; i < diff; ++i)
			{
				PushBack(value);
			}
		}
	}

	template<typename T>
	inline Vector<T>::iterator Vector<T>::PushBack(const_reference value)
	{
		return EmplaceBack(value);
	}

	template<typename T>
	inline Vector<T>::iterator Vector<T>::PushBack(rvalue_reference value)
	{
		return EmplaceBack(std::move(value));
	}

	template<typename T>
	template<typename... Args>
	inline Vector<T>::iterator Vector<T>::EmplaceBack(Args... args)
	{
		if (_size == _capacity)
		{
			size_t newCapacity = std::max(_reserveStrategy(_capacity), _capacity + 1);
			Reserve(newCapacity);
		}
		new (_data + _size) value_type(std::forward<Args>(args)...);
		++_size;
		return iterator(*this, _data + _size - 1);
	}

	template<typename T>
	inline Vector<T>& Vector<T>::Append(const Vector& other)
	{
		Reserve(_size + other._size);
		for (const auto& item : other)
		{
			PushBack(item);
		}
		return *this;
	}

	template<typename T>
	typename Vector<T>::iterator Vector<T>::Find(const_reference value, EqualityFunctor equalityFunctor)
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

	template<typename T>
	typename Vector<T>::const_iterator Vector<T>::Find(const_reference value, EqualityFunctor equalityFunctor) const
	{
		const_iterator it = cbegin();
		for (; it != end(); ++it)
		{
			if (equalityFunctor(*it, value))
			{
				break;
			}
		}
		return it;
	}

	template<typename T>
	inline void Vector<T>::Clear()
	{
		for (size_t i = 0; i < _size; i++)
		{
			_data[i].~T();
		}
		--_size = 0;
	}

	template<typename T>
	inline void Vector<T>::ShrinkToFit()
	{
		if (_size == 0)
		{
			free(_data);
			_data = nullptr;
			_capacity = 0;
		}
		else if (_capacity > _size && _capacity != 0)
		{
			pointer data = reinterpret_cast<pointer>(std::realloc(_data, _size * sizeof(value_type)));
			assert(data != nullptr);
			_data = data;
			_capacity = _size;
		}
	}

	template<typename T>
	inline bool Vector<T>::Remove(const_reference value, EqualityFunctor equalityFunctor)
	{
		return Remove(Find(value, equalityFunctor));
	}

	template<typename T>
	inline bool Vector<T>::Remove(const iterator& first)
	{
		return Remove(first, first + 1);
	}

	template<typename T>
	inline bool Vector<T>::Remove(const iterator& first, const iterator& last)
	{
		if (first._container != this || last._container != this)
		{
			return false;
		}

		iterator firstCpy = first;
		iterator lastCpy = last;

		iterator beginIt = begin();
		iterator endIt = end();

		if (firstCpy < beginIt)
		{
			firstCpy = beginIt;
		}

		if (lastCpy > endIt)
		{
			lastCpy = endIt;
		}

		if (firstCpy >= endIt || lastCpy <= beginIt || firstCpy >= lastCpy)
		{
			return false;
		}

		for (const_iterator it = firstCpy; it != lastCpy; ++it)
		{
			it->~T();
		}
		std::size_t elementsToMove = end() - lastCpy;
		if (elementsToMove > 0)
		{
			std::memmove(&*firstCpy, &*lastCpy, elementsToMove * sizeof(T));
		}
		_size -= lastCpy - firstCpy;
		return true;
	}
#pragma endregion Vector

#pragma region const_iterator
	template <typename T>
	Vector<T>::const_iterator::const_iterator(const Vector& container, pointer ptr) :
		_container(&container), _ptr(ptr)
	{
	}

	template <typename T>
	Vector<T>::const_iterator::const_iterator(const iterator& other) :
		_container(other._container), _ptr(other._ptr)
	{
	}

	template <typename T>
	inline typename Vector<T>::const_reference Vector<T>::const_iterator::operator*() const
	{
		if (_ptr == nullptr)
		{
			throw std::runtime_error("iterator points to null");
		}
		if (_ptr < _container->_data || _ptr >= _container->_data + _container->Size())
		{
			throw std::runtime_error("iterator out of bounds");
		}
		return *_ptr;
	}

	template <typename T>
	inline const typename Vector<T>::const_iterator::value_type* Vector<T>::const_iterator::operator->() const
	{
		return &operator*();
	}

	template<typename T>
	inline Vector<T>::const_reference Vector<T>::const_iterator::operator[](std::size_t index) const
	{
		if (_container == nullptr)
		{
			throw std::runtime_error("Iterator is not associated with a container");
		}
		return _container->At(index);
	}

	template <typename T>
	inline typename Vector<T>::const_iterator& Vector<T>::const_iterator::operator++()
	{
		if (_container == nullptr)
		{
			throw std::runtime_error("Iterator is not associated with a container");
		}

		if (_ptr != nullptr)
		{
			++_ptr;
		}
		return *this;
	}

	template <typename T>
	inline typename Vector<T>::const_iterator Vector<T>::const_iterator::operator++(int)
	{
		const_iterator it = *this;
		operator++();
		return it;
	}

	template <typename T>
	inline typename Vector<T>::const_iterator& Vector<T>::const_iterator::operator--()
	{
		if (_container == nullptr)
		{
			throw std::runtime_error("Iterator is not associated with a container");
		}

		if (_ptr != nullptr)
		{
			--_ptr;
		}
		return *this;
	}

	template <typename T>
	inline typename Vector<T>::const_iterator Vector<T>::const_iterator::operator--(int)
	{
		const_iterator it = *this;
		operator--();
		return it;
	}

	template <typename T>
	inline typename Vector<T>::const_iterator& Vector<T>::const_iterator::operator+=(size_t n)
	{
		if (_container == nullptr)
		{
			throw std::runtime_error("Iterator is not associated with a container");
		}

		if (_ptr != nullptr)
		{
			_ptr += n;
		}
		return *this;
	}

	template <typename T>
	inline typename Vector<T>::const_iterator Vector<T>::const_iterator::operator+(size_t n) const
	{
		const_iterator it = *this;
		it += n;
		return it;
	}

	template <typename T>
	inline typename Vector<T>::const_iterator& Vector<T>::const_iterator::operator-=(size_t n)
	{
		if (_container == nullptr)
		{
			throw std::runtime_error("Iterator is not associated with a container");
		}

		if (_ptr != nullptr)
		{
			_ptr -= n;
		}
		return *this;
	}

	template <typename T>
	inline typename Vector<T>::const_iterator Vector<T>::const_iterator::operator-(size_t n) const
	{
		const_iterator it = *this;
		it -= n;
		return it;
	}

	template<typename T>
	inline size_t Vector<T>::const_iterator::operator-(const const_iterator& it) const
	{
		if (_container != it._container)
		{
			throw std::runtime_error("Iterators are from different containers");
		}
		return _ptr - it._ptr;
	}

	template<typename T>
	inline bool Vector<T>::const_iterator::operator<(const const_iterator& it) const
	{
		if (_container != it._container)
		{
			throw std::runtime_error("Iterators are from different containers");
		}
		return _ptr < it._ptr;
	}

	template<typename T>
	inline bool Vector<T>::const_iterator::operator<=(const const_iterator& it) const
	{
		if (_container != it._container)
		{
			throw std::runtime_error("Iterators are from different containers");
		}
		return _ptr <= it._ptr;
	}

	template<typename T>
	inline bool Vector<T>::const_iterator::operator>(const const_iterator& it) const
	{
		if (_container != it._container)
		{
			throw std::runtime_error("Iterators are from different containers");
		}
		return _ptr > it._ptr;
	}

	template<typename T>
	inline bool Vector<T>::const_iterator::operator>=(const const_iterator& it) const
	{
		if (_container != it._container)
		{
			throw std::runtime_error("Iterators are from different containers");
		}
		return _ptr >= it._ptr;
	}

	template <typename T>
	inline bool Vector<T>::const_iterator::operator==(const const_iterator& other) const
	{
		return !operator!=(other);
	}

	template <typename T>
	inline bool Vector<T>::const_iterator::operator!=(const const_iterator& other) const
	{
		return (_ptr != other._ptr) || (_container != other._container);
	}
#pragma endregion const_iterator

#pragma region iterator
	template <typename T>
	Vector<T>::iterator::iterator(Vector& container, pointer ptr) :
		_container(&container), _ptr(ptr)
	{
	}

	template <typename T>
	inline Vector<T>::reference Vector<T>::iterator::operator*() const
	{
		if (_ptr == nullptr)
		{
			throw std::runtime_error("iterator points to null");
		}
		if (_ptr < _container->_data || _ptr >= _container->_data + _container->Size())
		{
			throw std::runtime_error("iterator out of bounds");
		}
		return *_ptr;
	}

	template <typename T>
	inline typename Vector<T>::pointer Vector<T>::iterator::operator->() const
	{
		return &operator*();
	}

	template<typename T>
	inline Vector<T>::reference Vector<T>::iterator::operator[](std::size_t index) const
	{
		if (_container == nullptr)
		{
			throw std::runtime_error("Iterator is not associated with a container");
		}
		return _container->At(index);
	}

	template <typename T>
	inline typename Vector<T>::iterator& Vector<T>::iterator::operator++()
	{
		if (_container == nullptr)
		{
			throw std::runtime_error("Iterator is not associated with a container");
		}

		if (_ptr != nullptr)
		{
			++_ptr;
		}
		return *this;
	}

	template <typename T>
	inline typename Vector<T>::iterator Vector<T>::iterator::operator++(int)
	{
		iterator it = *this;
		operator++();
		return it;
	}

	template <typename T>
	inline typename Vector<T>::iterator& Vector<T>::iterator::operator--()
	{
		if (_container == nullptr)
		{
			throw std::runtime_error("Iterator is not associated with a container");
		}

		if (_ptr != nullptr)
		{
			--_ptr;
		}
		return *this;
	}

	template <typename T>
	inline typename Vector<T>::iterator Vector<T>::iterator::operator--(int)
	{
		iterator it = *this;
		operator--();
		return it;
	}

	template <typename T>
	inline typename Vector<T>::iterator& Vector<T>::iterator::operator+=(size_t n)
	{
		if (_container == nullptr)
		{
			throw std::runtime_error("Iterator is not associated with a container");
		}

		if (_ptr != nullptr)
		{
			_ptr += n;
		}
		return *this;
	}

	template <typename T>
	inline typename Vector<T>::iterator Vector<T>::iterator::operator+(size_t n) const
	{
		iterator it = *this;
		it += n;
		return it;
	}

	template <typename T>
	inline typename Vector<T>::iterator& Vector<T>::iterator::operator-=(size_t n)
	{
		if (_container == nullptr)
		{
			throw std::runtime_error("Iterator is not associated with a container");
		}

		if (_ptr != nullptr)
		{
			_ptr -= n;
		}
		return *this;
	}

	template <typename T>
	typename Vector<T>::iterator Vector<T>::iterator::operator-(size_t n) const
	{
		iterator it = *this;
		it -= n;
		return it;
	}

	template<typename T>
	inline size_t Vector<T>::iterator::operator-(const iterator& it) const
	{
		if (_container != it._container)
		{
			throw std::runtime_error("Iterators are from different containers");
		}
		return _ptr - it._ptr;
	}

	template<typename T>
	inline bool Vector<T>::iterator::operator<(const iterator& it) const
	{
		if (_container != it._container)
		{
			throw std::runtime_error("Iterators are from different containers");
		}
		return _ptr < it._ptr;
	}

	template<typename T>
	inline bool Vector<T>::iterator::operator<=(const iterator& it) const
	{
		if (_container != it._container)
		{
			throw std::runtime_error("Iterators are from different containers");
		}
		return _ptr <= it._ptr;
	}

	template<typename T>
	inline bool Vector<T>::iterator::operator>(const iterator& it) const
	{
		if (_container != it._container)
		{
			throw std::runtime_error("Iterators are from different containers");
		}
		return _ptr > it._ptr;
	}

	template<typename T>
	inline bool Vector<T>::iterator::operator>=(const iterator& it) const
	{
		if (_container != it._container)
		{
			throw std::runtime_error("Iterators are from different containers");
		}
		return _ptr >= it._ptr;
	}

	template <typename T>
	inline bool Vector<T>::iterator::operator==(const iterator& other) const
	{
		return !operator!=(other);
	}

	template <typename T>
	inline bool Vector<T>::iterator::operator!=(const iterator& other) const
	{
		return (_ptr != other._ptr) || (_container != other._container);
	}
#pragma endregion iterator
}