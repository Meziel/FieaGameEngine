#include "Datum.h"
#include <utility>
#include <stdexcept>

namespace FieaGameEngine
{
	template <VariantDatumType... Types>
	inline VariantDatum<Types...>::VariantDatum(DatumTypes type) :
		_type(type)
	{
	}

	template <VariantDatumType... Types>
	template <VariantDatumType T>
	inline VariantDatum<Types...>::VariantDatum(const T& value) : VariantDatum(ToDatumType<T>())
	{
		PushBack(value);
	}

	template <VariantDatumType... Types>
	template <VariantDatumType T>
	inline VariantDatum<Types...>::VariantDatum(T&& value) : VariantDatum(ToDatumType<T>())
	{
		PushBack(std::move(value));
	}

	template<VariantDatumType... Types>
	inline VariantDatum<Types...>::VariantDatum(const VariantDatum& other) :
		_type(other._type),
		_isExternal(other._isExternal)
	{
		if (_isExternal)
		{
			_capacity = other._capacity;
			_size = other._size;
			_data = other._data;
		}
		else
		{
			Reserve(other._size);
			Visit([&]<VariantDatumType T>()
			{
				T* const otherDataTyped = reinterpret_cast<T*>(other._data);
				for (size_t i = 0; i < other._size; ++i)
				{
					PushBack(otherDataTyped[i]);
				}
			});
		}
	}

	template<VariantDatumType... Types>
	inline VariantDatum<Types...>::VariantDatum(VariantDatum&& other) noexcept :
		_capacity(other._capacity),
		_size(other._size),
		_type(other._type),
		_data(other._data)
	{
		other._capacity = 0;
		other._size = 0;
		other._data = nullptr;
	}

	template <VariantDatumType... Types>
	template <VariantDatumType T>
	inline VariantDatum<Types...>::VariantDatum(T* data, std::size_t size)
	{
		SetStorage(data, size);
	}

	template <VariantDatumType... Types>
	template <VariantDatumType T>
	inline VariantDatum<Types...>::VariantDatum(std::initializer_list<T> list) : VariantDatum(ToDatumType<T>())
	{
		Reserve(list.size());
		for (const T& value : list)
		{
			PushBack(value);
		}
	}

	template<VariantDatumType... Types>
	inline VariantDatum<Types...>::~VariantDatum()
	{
		if (!_isExternal)
		{
			Clear();
			ShrinkToFit();
		}
	}

	template<VariantDatumType... Types>
	inline VariantDatum<Types...>& VariantDatum<Types...>::operator=(const VariantDatum& rhs)
	{
		// Delete old
		if (!_isExternal)
		{
			Clear();
			ShrinkToFit();
		}
		else
		{
			_capacity = 0;
			_size = 0;
			_isExternal = false;
			_data = nullptr;
		}

		// Copy
		_type = rhs._type;
		if (rhs._isExternal)
		{
			_data = rhs._data;
			_size = rhs._size;
			_capacity = rhs._capacity;
		}
		else
		{
			Clear();
			Reserve(rhs._size);
			Visit([&]<VariantDatumType T>()
			{
				T* const otherDataTyped = reinterpret_cast<T*>(rhs._data);
				for (size_t i = 0; i < rhs._size; ++i)
				{
					PushBack(otherDataTyped[i]);
				}
			});
		}
		_isExternal = rhs._isExternal;
		return *this;
	}

	template<VariantDatumType... Types>
	inline VariantDatum<Types...>& VariantDatum<Types...>::operator=(VariantDatum&& rhs) noexcept
	{
		_capacity = rhs._capacity;
		_size = rhs._size;
		_type = rhs._type;
		_data = rhs._data;

		rhs._capacity = 0;
		rhs._size = 0;
		rhs._data = nullptr;
		return *this;
	}

	template <VariantDatumType... Types>
	template<VariantDatumType T>
	inline VariantDatum<Types...>& VariantDatum<Types...>::operator=(const T& rhs)
	{
		// Delete old
		if (!_isExternal)
		{
			Clear();
			ShrinkToFit();
		}
		else
		{
			_capacity = 0;
			_size = 0;
			_isExternal = false;
			_data = nullptr;
		}

		_type = ToDatumType<T>();
		_isExternal = false;
		PushBack(rhs);
		return *this;
	}

	template <VariantDatumType... Types>
	template<VariantDatumType T>
	inline VariantDatum<Types...>& VariantDatum<Types...>::operator=(T&& rhs)
	{
		// Delete old
		if (!_isExternal)
		{
			Clear();
			ShrinkToFit();
		}
		else
		{
			_capacity = 0;
			_size = 0;
			_isExternal = false;
			_data = nullptr;
		}

		_type = ToDatumType<T>();
		_isExternal = false;
		PushBack(rhs);
		return *this;
	}

	template<VariantDatumType... Types>
	inline DatumTypes VariantDatum<Types...>::Type() const
	{
		return _type;
	}

	template<VariantDatumType... Types>
	inline void VariantDatum<Types...>::SetType(DatumTypes type)
	{
		if (type == DatumTypes::Unknown && _type != DatumTypes::Unknown)
		{
			throw std::runtime_error("Type cannot be set to unknown");
		}

		if (_type != DatumTypes::Unknown && type != _type)
		{
			throw std::runtime_error("Cannot change to different type");
		}

		_type = type;
	}

	template<VariantDatumType... Types>
	inline std::size_t VariantDatum<Types...>::Size() const
	{
		return _size;
	}

	template<VariantDatumType... Types>
	inline std::size_t VariantDatum<Types...>::Capacity() const
	{
		return _capacity;
	}

	template<VariantDatumType... Types>
	inline bool VariantDatum<Types...>::IsEmpty() const
	{
		return _size == 0;
	}

	template<VariantDatumType... Types>
	inline void VariantDatum<Types...>::Reserve(std::size_t capacity)
	{
		if (capacity == 0)
		{
			return;
		}

		if (_isExternal)
		{
			throw std::runtime_error("Datum is external");
		}

		if (_type == DatumTypes::Unknown)
		{
			throw std::runtime_error("Type is unknown");
		}

		if (capacity > _capacity)
		{
			Visit([&]<VariantDatumType T>()
			{
				std::byte* const data = reinterpret_cast<std::byte*>(realloc(_data, capacity * sizeof(T)));
				assert(data != nullptr);
				_data = data;
			});
			_capacity = capacity;
		}
	}

	template<VariantDatumType... Types>
	inline void VariantDatum<Types...>::Resize(std::size_t size)
	{
		size;
		if (_isExternal)
		{
			throw std::runtime_error("Cannot resize external datum");
		}

		if (_type == DatumTypes::Unknown)
		{
			throw std::runtime_error("Type is unknown");
		}

		// No size change
		if (size == _size)
		{
			return;
		}
		// Shrinking
		if (size < _size)
		{
			const size_t diff = _size - size;
			for (size_t i = 0; i < diff; ++i)
			{
				RemoveAt(_size - 1);
			}
		}
		// Growing
		else
		{
			const size_t diff = size - _size;
			Reserve(size);
			Visit([&]<VariantDatumType T>()
			{
				for (size_t i = 0; i < diff; ++i)
				{
					PushBack(T{});
				}
			});
		}
		ShrinkToFit();
	}

	template <VariantDatumType... Types>
	template <VariantDatumType T>
	inline void VariantDatum<Types...>::Resize(std::size_t size, const T& value)
	{
		if (_type == DatumTypes::Unknown)
		{
			throw std::runtime_error("Type unknown");
		}

		if (_type != ToDatumType<T>())
		{
			throw std::runtime_error("Provided prototype type does not match datum's type");
		}

		if (size == _size)
		{
			return;
		}

		if (size < _size)
		{
			const size_t diff = _size - size;
			for (size_t i = 0; i < diff; ++i)
			{
				RemoveAt(_size - 1);
			}
		}
		else
		{
			const size_t diff = size - _size;
			Reserve(size);
			for (size_t i = 0; i < diff; ++i)
			{
				PushBack(value);
			}
		}
		// Matches capacity to size
		ShrinkToFit();
	}

	template<VariantDatumType... Types>
	inline void VariantDatum<Types...>::Clear()
	{
		if (_isExternal)
		{
			throw std::runtime_error("Cannot clear external datum");
		}

		if (_type != DatumTypes::Unknown)
		{
			Visit([&]<VariantDatumType T>()
			{
				if (!std::is_trivially_destructible_v<T>)
				{
					T* const dataTyped = reinterpret_cast<T*>(_data);
					for (size_t i = 0; i < _size; ++i)
					{
						dataTyped[i].~T();
					}
				}
			});
		}
		--_size = 0;
	}

	template<VariantDatumType... Types>
	inline void VariantDatum<Types...>::ShrinkToFit()
	{
		if (_isExternal)
		{
			throw std::runtime_error("Cannot shrink external datum");
		}

		if (_size == 0)
		{
			free(_data);
			_data = nullptr;
			_capacity = 0;
		}
		else if (_capacity > _size)
		{
			Visit([&]<VariantDatumType T>()
			{
				std::byte* const data = reinterpret_cast<std::byte*>(std::realloc(_data, _size * sizeof(T)));
				assert(data != nullptr);
				_data = data;
			});

			_capacity = _size;
		}
	}

	template <VariantDatumType... Types>
	template <VariantDatumType T>
	inline void VariantDatum<Types...>::SetStorage(T* data, std::size_t size)
	{
		DatumTypes type = ToDatumType<T>();
		SetStorage(type, data, size);
	}

	template<VariantDatumType... Types>
	inline bool VariantDatum<Types...>::IsExternal()
	{
		return _isExternal;
	}

	template<VariantDatumType... Types>
	inline bool VariantDatum<Types...>::operator==(const VariantDatum& rhs) const
	{
		if (_size != rhs._size || _type != rhs._type)
		{
			return false;
		}

		if (_type == DatumTypes::Unknown)
		{
			return true;
		}

		bool equal = false;
		Visit([&]<VariantDatumType T>()
		{
			DefaultEquality<T> equalityFunctor{};
			T* const dataTyped = reinterpret_cast<T*>(_data);
			T* const rhsDataTyped = reinterpret_cast<T*>(rhs._data);
			for (std::size_t i = 0; i < _size; ++i)
			{
				if (!equalityFunctor(dataTyped[i], rhsDataTyped[i]))
				{
					equal = false;
					return;
				}
			}
			equal = true;
		});

		return equal;
	}

	template<VariantDatumType... Types>
	inline bool VariantDatum<Types...>::operator!=(const VariantDatum& rhs) const
	{
		return !operator==(rhs);
	}

	template <VariantDatumType... Types>
	template<VariantDatumType T>
	inline bool VariantDatum<Types...>::operator==(const T& rhs) const
	{
		if (_size != 1)
		{
			return false;
		}

		return Get<T>() == rhs;
	}

	template <VariantDatumType... Types>
	template<VariantDatumType T>
	inline bool VariantDatum<Types...>::operator!=(const T& rhs) const
	{
		return operator!=(rhs);
	}

	template <VariantDatumType... Types>
	template <VariantDatumType T>
	inline void VariantDatum<Types...>::Set(const T& value, std::size_t index)
	{
		if (index >= _size)
		{
			throw std::runtime_error("Index out of bounds");
		}

		if (_type != ToDatumType<T>())
		{
			throw std::runtime_error("Types does not match datum type");
		}

		reinterpret_cast<T*>(_data)[index] = T(value);
	}

	template <VariantDatumType... Types>
	template <VariantDatumType T>
	inline void VariantDatum<Types...>::Set(T&& value, std::size_t index)
	{
		if (index >= _size)
		{
			throw std::runtime_error("Index out of bounds");
		}

		if (_type != ToDatumType<T>())
		{
			throw std::runtime_error("Types does not match datum type");
		}
		
		reinterpret_cast<T*>(_data)[index] = T(std::forward<T>(value));
	}

	template <VariantDatumType... Types>
	template <VariantDatumType T>
	inline T& VariantDatum<Types...>::Get(std::size_t index)
	{
		if (_type != ToDatumType<T>())
		{
			throw std::runtime_error("Invalid type");
		}

		if (index >= _size)
		{
			throw std::runtime_error("Index out of bounds");
		}

		return reinterpret_cast<T*>(_data)[index];
	}

	template <VariantDatumType... Types>
	template <VariantDatumType T>
	inline const T& VariantDatum<Types...>::Get(std::size_t index) const
	{
		if (_type != ToDatumType<T>())
		{
			throw std::runtime_error("Invalid type");
		}

		if (index >= _size)
		{
			throw std::runtime_error("Index out of bounds");
		}

		return reinterpret_cast<T*>(_data)[index];
	}

	template <VariantDatumType... Types>
	template<VariantDatumType T>
	inline const T& VariantDatum<Types...>::operator[](std::size_t index) const
	{
		return Get<T>(index);
	}

	template <VariantDatumType... Types>
	template <VariantDatumType T>
	requires std::copy_constructible<T>
	inline void VariantDatum<Types...>::PushBack(const T& value)
	{
		EmplaceBack<T>(value);
	}

	template <VariantDatumType... Types>
	template <VariantDatumType T>
	inline void VariantDatum<Types...>::PushBack(T&& value)
	{
		EmplaceBack<T>(std::move(value));
	}

	template <VariantDatumType... Types>
	template <VariantDatumType T, typename... Args>
	inline void VariantDatum<Types...>::EmplaceBack(Args... args)
	{
		const DatumTypes type = ToDatumType<T>();

		if (_type == DatumTypes::Unknown)
		{
			SetType(type);
		}

		if (_type != type)
		{
			throw std::runtime_error("Datum is of a different type");
		}

		if (_isExternal)
		{
			throw std::runtime_error("Datum is external");
		}

		if (_size == _capacity)
		{
			size_t newCapacity = _capacity > 0 ? _capacity * 2 : 1;
			Reserve(newCapacity);
		}

		new (_data + _size * sizeof(T)) T(std::forward<Args>(args)...);
		++_size;
	}

	template<VariantDatumType... Types>
	inline void VariantDatum<Types...>::PopBack()
	{
		if (_size == 0)
		{
			throw std::runtime_error("Vector is empty");
		}

		if (_isExternal)
		{
			throw std::runtime_error("Datum is external");
		}

		Visit([&]<VariantDatumType T>()
		{
			if (!std::is_trivially_destructible_v<T>)
			{
				T* const dataTyped = reinterpret_cast<T*>(_data);
				dataTyped[_size - 1].~T();
			}
		});
		--_size;
	}

	template <VariantDatumType... Types>
	template<VariantDatumType T>
	inline T& VariantDatum<Types...>::Front()
	{
		if (_size == 0)
		{
			throw std::runtime_error("Datum is empty");
		}

		if (_type != ToDatumType<T>())
		{
			throw std::runtime_error("Invalid type");
		}

		return Get<T>(0);
	}

	template <VariantDatumType... Types>
	template<VariantDatumType T>
	inline const T& VariantDatum<Types...>::Front() const
	{
		if (_size == 0)
		{
			throw std::runtime_error("Datum is empty");
		}

		if (_type != ToDatumType<T>())
		{
			throw std::runtime_error("Invalid type");
		}

		return Get<T>(0);
	}

	template <VariantDatumType... Types>
	template<VariantDatumType T>
	inline T& VariantDatum<Types...>::Back()
	{
		if (_size == 0)
		{
			throw std::runtime_error("Datum is empty");
		}

		if (_type != ToDatumType<T>())
		{
			throw std::runtime_error("Invalid type");
		}

		return Get<T>(_size - 1);
	}

	template <VariantDatumType... Types>
	template<VariantDatumType T>
	inline const T& VariantDatum<Types...>::Back() const
	{
		if (_size == 0)
		{
			throw std::runtime_error("Datum is empty");
		}

		if (_type != ToDatumType<T>())
		{
			throw std::runtime_error("Invalid type");
		}

		return Get<T>(_size - 1);
	}

	template <VariantDatumType... Types>
	template<VariantDatumType T>
	inline bool VariantDatum<Types...>::Remove(const T& value)
	{
		if (ToDatumType<T>() != _type)
		{
			throw std::runtime_error("Invalid type");
		}
		return RemoveAt(Find(value));
	}

	template<VariantDatumType... Types>
	inline bool VariantDatum<Types...>::RemoveAt(std::size_t index)
	{
		if (_isExternal)
		{
			throw std::runtime_error("Storage is external");
		}

		if (index >= _size)
		{
			return false;
		}

		Visit([&]<VariantDatumType T>()
		{
			T* const dataTyped = reinterpret_cast<T*>(_data);
			if (!std::is_trivially_destructible_v<T>)
			{
				dataTyped[index].~T();
			}
			const std::size_t elementsToMove = _size - (index + 1);
			if (elementsToMove > 0)
			{
				std::memmove(dataTyped + index, dataTyped + index + 1, elementsToMove * sizeof(T));
			}
		});

		_size -= 1;
		return true;
	}

	template <VariantDatumType... Types>
	template<VariantDatumType T>
	inline std::size_t VariantDatum<Types...>::Find(const T& value)
	{
		for (size_t i = 0; i < _size; ++i)
		{
			const T& dataAtIndex = reinterpret_cast<T*>(_data)[i];
			if (dataAtIndex == value)
			{
				return i;
			}
		}
		return _size;
	}
	
	// Private SetStorage that uses void*
	template<VariantDatumType... Types>
	inline void VariantDatum<Types...>::SetStorage(DatumTypes type, void* data, std::size_t size)
	{
		if (_type != DatumTypes::Unknown && _type != type)
		{
			throw std::runtime_error("Cannot change types");
		}

		if (!_isExternal && _size != 0)
		{
			throw std::runtime_error("Datum already contains internal data");
		}

		SetType(type);
		_data = reinterpret_cast<std::byte*>(data);
		_size = size;
		_capacity = size;
		_isExternal = true;
	}

	template<VariantDatumType... Types>
	template <typename Func, typename... Args>
	inline void VariantDatum<Types...>::Visit(Func func, Args&&...args)
	{
		// Iteratively loop through types but short-circuit if found
		(... ||
			[&]() -> bool
			{
				if (_type == ToDatumType<Types>())
				{
					func.template operator() < Types > (std::forward<Args>(args)...);
					return true;
				}
				else
				{
					return false;
				}
			}()
		);
	}

	template<VariantDatumType... Types>
	template <typename Func, typename... Args>
	inline void VariantDatum<Types...>::Visit(const Func func, Args&& ...args) const
	{
		const_cast<VariantDatum*>(this)->Visit(func, std::forward<Args>()...);
	}
}