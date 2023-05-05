#pragma once

#include "RTTI.h"
#include "GameTime.h"

namespace FieaGameEngine
{
	struct EventBase : RTTI
	{
		RTTI_DECLARATIONS(EventBase, RTTI);

	public:
		virtual std::unique_ptr<EventBase> Clone() const = 0;
		virtual ~EventBase() = default;

	protected:
		EventBase() = default;
		EventBase(const EventBase&) = default;
		EventBase(EventBase&&) noexcept = default;
		EventBase& operator=(const EventBase&) = default;
		EventBase& operator=(EventBase&&) noexcept = default;
	};

	template<typename T>
	struct Event final : EventBase
	{
		RTTI_DECLARATIONS(Event, EventBase);

	public:
		/// <summary>
		/// Constructor
		/// </summary>
		/// <param name="message">Message data associated with this event</param>
		Event(T message);

		/// <summary>
		/// Gets the message data associated with this event
		/// </summary>
		/// <returns>Message data</returns>
		const T& Message() const;

		std::unique_ptr<EventBase> Clone() const override
		{
			return std::make_unique<Event>(*this);
		};

	private:
		T _message;
	};
}

#include "Event.inl"