#include "EventPublisher.h"

namespace FieaGameEngine
{
	template <std::derived_from<EventBase> EventType>
	void EventPublisher::Subscribe(IEventSubscriber& subscriber)
	{
		_pendingSubscribeInstructions.EmplaceBack(PendingSubscriptionInstructionTypes::SUBSCRIBE, std::pair<IdType, IEventSubscriber&>{ EventType::TypeIdClass(), subscriber });
		if (!_isQueueUpdating)
		{
			_ProcessPendingSubscribeInstructions();
		}
	}

	template <std::derived_from<EventBase> EventType>
	void EventPublisher::Unsubscribe(IEventSubscriber& subscriber)
	{
		_pendingSubscribeInstructions.EmplaceBack(PendingSubscriptionInstructionTypes::UNSUBSCRIBE, std::pair<IdType, IEventSubscriber&>{ EventType::TypeIdClass(), subscriber });
		if (!_isQueueUpdating)
		{
			_ProcessPendingSubscribeInstructions();
		}
	}

	inline bool DefaultEquality<std::reference_wrapper<IEventSubscriber>>::operator()(const std::reference_wrapper<IEventSubscriber>& lhs, const std::reference_wrapper<IEventSubscriber>& rhs) const
	{
		return &lhs.get() == &rhs.get();
	}
}
