#include "pch.h"
//
//#include "EventQueue.h"
//#include "GameTime.h"
//
//namespace FieaGameEngine
//{
//	inline void EventQueue::Enqueue(EventPublisher& eventPublisher, std::chrono::milliseconds delay, const GameTime& gameTime)
//	{
//		_pendingInstructions.PushBack({ QueueInstructionTypes::ADD, QueueItem{gameTime.CurrentTime(), delay, eventPublisher }});
//	}
//
//	inline void EventQueue::Update(const GameTime& gameTime)
//	{
//		// Update queue with pending instructions
//		for (const auto& pendingInstruction : _pendingInstructions)
//		{
//			switch (pendingInstruction.InstructionType)
//			{
//				case QueueInstructionTypes::ADD:
//					_queue.PushBack(std::get<QueueItem>(pendingInstruction.Data));
//					break;
//				case QueueInstructionTypes::CLEAR:
//					_queue.Clear();
//					break;
//				default:
//					break;
//			}
//		}
//		_pendingInstructions.Clear();
//
//		// Deliver items in queue that expired
//		for (const QueueItem& queueItem : _queue)
//		{
//			if (queueItem.IsExpired(gameTime))
//			{
//				queueItem.eventPublisher.Deliver();
//				// TODO: remove from queue
//			}
//		}
//	}
//
//	void EventQueue::Clear()
//	{
//		_pendingInstructions.PushBack({QueueInstructionTypes::CLEAR});
//	}
//
//	bool EventQueue::IsEmpty()
//	{
//		return _queue.IsEmpty();
//	}
//
//	std::size_t EventQueue::Size()
//	{
//		return _queue.Size();
//	}
//
//	bool EventQueue::QueueItem::IsExpired(const GameTime& gameTime) const
//	{
//		return gameTime.CurrentTime() > TimeEnqueued + Delay;
//	}
//}
