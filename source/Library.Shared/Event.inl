#include "Event.h"

namespace FieaGameEngine
{
    template <typename T>
    RTTI_DEFINITIONS(Event<T>);

	template <typename T>
	Event<T>::Event(T message) :
		_message(std::move(message))
	{
	}

	template<typename T>
	const T& Event<T>::Message() const
	{
		return _message;
	}
}