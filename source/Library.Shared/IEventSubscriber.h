#pragma once

#include "Event.h"

namespace FieaGameEngine
{
	class IEventSubscriber
	{
	public:
		virtual ~IEventSubscriber() = default;
		virtual void Notify(const EventBase& e) noexcept = 0;
	};
}
