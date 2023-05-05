#include "pch.h"
#include "EventPublisher.h"

#include "IEventSubscriber.h"

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(EventPublisher);

	void EventPublisher::Enqueue(const EventBase& e, const GameTime& gameTime, std::chrono::milliseconds delay)
	{
		_pendingQueueInstructions.EmplaceBack(PendingQueueInstructionTypes::ADD, QueuedEvent{ gameTime.CurrentTime(), delay, e.Clone() });
		if (!_isQueueUpdating)
		{
			_ProcessPendingQueueInstructions();
		}
	}

	void EventPublisher::Clear()
	{
		_pendingQueueInstructions.EmplaceBack(PendingQueueInstructionTypes::CLEAR);
		if (!_isQueueUpdating)
		{
			_ProcessPendingQueueInstructions();
		}
	}

	bool EventPublisher::IsEmpty() const
	{
		return _queuedEvents.IsEmpty();
	}

	std::size_t EventPublisher::Size() const
	{
		return _queuedEvents.Size();
	}

	void EventPublisher::Update(GameTime& gameTime) noexcept
	{
		_isQueueUpdating = true;

		// Deliver queued events
		for (const QueuedEvent& queuedEvent : _queuedEvents)
		{
			if (queuedEvent.IsExpired(gameTime))
			{
				_Deliver(*queuedEvent.Event);
			}		
		}
		auto it = std::partition(_queuedEvents.begin(), _queuedEvents.end(), [&gameTime](QueuedEvent qe) { return !qe.IsExpired(gameTime); });
		_queuedEvents.Remove(it, _queuedEvents.end());

		_ProcessPendingSubscribeInstructions();
		_ProcessPendingQueueInstructions();

		// TODO: find a better way to do this
		_isQueueUpdating = false;
	}

	void EventPublisher::_Deliver(const EventBase& e)
	{		
		auto it = _subscribers.Find(e.TypeIdInstance());
		if (it != _subscribers.end())
		{
			for (IEventSubscriber& subscriber : it->second)
			{
				subscriber.Notify(e);
			}
		}
	}

	void EventPublisher::_ProcessPendingSubscribeInstructions()
	{
		for (auto& pendingSubscribeInstruction : _pendingSubscribeInstructions)
		{
			auto& subscriptionData = std::get<std::pair<IdType, IEventSubscriber&>>(pendingSubscribeInstruction.Data);

			switch (pendingSubscribeInstruction.InstructionType)
			{
				case PendingSubscriptionInstructionTypes::SUBSCRIBE:
					_subscribers[subscriptionData.first].PushBack(subscriptionData.second);
					break;
				case PendingSubscriptionInstructionTypes::UNSUBSCRIBE:
				{
					auto it = _subscribers.Find(subscriptionData.first);
					if (it != _subscribers.end())
					{
						it->second.Remove(subscriptionData.second);
					}
					break;
				}
				default:
					break;
			}
		}
		_pendingSubscribeInstructions.Clear();
	}

	void EventPublisher::_ProcessPendingQueueInstructions()
	{
		for (auto& pendingQueueInstruction : _pendingQueueInstructions)
		{
			switch (pendingQueueInstruction.InstructionType)
			{
				case PendingQueueInstructionTypes::ADD:
					{
						auto& queuedEvent = std::get<QueuedEvent>(pendingQueueInstruction.Data);
						_queuedEvents.PushBack(queuedEvent);
					}
					break;
				case PendingQueueInstructionTypes::CLEAR:
					_queuedEvents.Clear();
					break;
				default:
					break;
			}
		}
		_pendingQueueInstructions.Clear();
	}

	bool EventPublisher::QueuedEvent::IsExpired(const GameTime& gameTime) const
	{
		return gameTime.CurrentTime() >= TimeEnqueued + Delay;
	}

	void swap(EventPublisher::QueuedEvent& left, EventPublisher::QueuedEvent& right)
	{
		using std::swap;
		swap(left.TimeEnqueued, right.TimeEnqueued);
		swap(left.Delay, right.Delay);
		swap(left.Event, right.Event);
	}
}