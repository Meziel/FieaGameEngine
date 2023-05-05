#pragma once

#include <concepts>
#include <variant>
#include "RTTI.h"
#include "HashMap.h"
#include "Vector.h"
#include "Event.h"
#include "GameClock.h"

namespace FieaGameEngine
{
	class IEventSubscriber;

	class EventPublisher final : RTTI
	{
		RTTI_DECLARATIONS(EventPublisher, RTTI);
	public:
		/// <summary>
		/// Adds a subscriber for a specified event. When the event happens,
		/// the subscribers Notify method is invoked.
		/// If the update method is running, the subscriber will be added after the update method is complete.
		/// </summary>
		/// <typeparam name="EventType">Event type subscribe to</typeparam>
		/// <param name="subscriber">Entity to subscribe</param>
		template <std::derived_from<EventBase> EventType>
		void Subscribe(IEventSubscriber& subscriber);

		/// <summary>
		/// Removes a subscriber for a specified event. If the update method is running, the subscriber will
		/// be removed after the update method is complete.
		/// </summary>
		/// <typeparam name="EventType"></typeparam>
		template <std::derived_from<EventBase> EventType>
		void Unsubscribe(IEventSubscriber& subscriber);

		/// <summary>
		/// Adds an event to be queued. If the update method is running, then the event will get queued after
		/// update method is complete.
		/// </summary>
		/// <param name="e">Event to add to the queue</param>
		/// <param name="gameTime">Current time</param>
		/// <param name="delay">Time in miliseconds until the event should be fired</param>
		void Enqueue(const EventBase& e, const GameTime& gameTime, std::chrono::milliseconds delay = {});

		/// <summary>
		/// Clears the event queue. If the rupdate method is running, the even queue will be cleared after
		/// the update method is complete.
		/// </summary>
		void Clear();

		/// <summary>
		/// Returns whether the event queue is empty. This does not account for pending submissions to the queue.
		/// </summary>
		/// <returns></returns>
		bool IsEmpty() const;

		/// <summary>
		/// Returns the size of the event queue. This does not include pending submissions to the queue.
		/// </summary>
		/// <returns>Size of the event queue</returns>
		std::size_t Size() const;

		/// <summary>
		/// Iterates through all events in the event queue and notifies subscribers of the events.
		/// If the subscribers or event queue is modified during the update method, the operations are added as pending instructions
		/// After the update is complete, pending instructions will be applied to the subscriber list and event queue.
		/// </summary>
		/// <param name="gameTime">Curent game time</param>
		void Update(GameTime& gameTime) noexcept;
	private:

		bool _isQueueUpdating = false;

		void _Deliver(const EventBase& e);
		void _ProcessPendingSubscribeInstructions();
		void _ProcessPendingQueueInstructions();

		struct QueuedEvent
		{
			std::chrono::high_resolution_clock::time_point TimeEnqueued;
			std::chrono::milliseconds Delay;
			std::shared_ptr<EventBase> Event;

			bool IsExpired(const GameTime& gameTime) const;

			friend void swap(QueuedEvent& left, QueuedEvent& right);
		};
		friend void swap(QueuedEvent& left, QueuedEvent& right);

		enum class PendingQueueInstructionTypes
		{
			CLEAR,
			ADD,
			REMOVE
		};

		enum class PendingSubscriptionInstructionTypes
		{
			SUBSCRIBE,
			UNSUBSCRIBE
		};

		struct PendingQueueInstruction
		{
			PendingQueueInstructionTypes InstructionType;
			std::variant<std::monostate, QueuedEvent> Data;
		};

		struct PendingSubscriptionInstruction
		{
			PendingSubscriptionInstructionTypes InstructionType;
			std::variant<std::monostate, std::pair<IdType, IEventSubscriber&>> Data;
		};

		Vector<PendingSubscriptionInstruction> _pendingSubscribeInstructions; 
		HashMap<IdType, Vector<std::reference_wrapper<IEventSubscriber>>> _subscribers;

		Vector<PendingQueueInstruction> _pendingQueueInstructions;
		Vector<QueuedEvent> _queuedEvents;
	};

	template<>
	struct DefaultEquality<std::reference_wrapper<IEventSubscriber>> final
	{
		inline bool operator()(const std::reference_wrapper<IEventSubscriber>& lhs, const std::reference_wrapper<IEventSubscriber>& rhs) const;
	};
}

#include "EventPublisher.inl"