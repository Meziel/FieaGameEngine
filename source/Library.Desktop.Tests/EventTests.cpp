#include "pch.h"
#include "CppUnitTest.h"
#include "GameObject.h"
#include "EventPublisher.h"
#include "Event.h"
#include "IEventSubscriber.h"
#include "ToStringSpecializations.h"
#include "Literals.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace UnitTests;
using namespace std;
using namespace std::string_literals;
using namespace std::chrono_literals;
using namespace FieaGameEngine;

namespace UnitTests
{
	struct TestSubscriber : public IEventSubscriber
	{
		virtual ~TestSubscriber() = default;

		inline void Notify(const EventBase& e) noexcept override
		{
			++NotifiesReceived;
			if (e.TypeIdInstance() == Event<Foo>::TypeIdClass())
			{
				++FooEventsReceived;
				LastFooMessageReceived = e.As<Event<Foo>>()->Message();
			}
		}

		int32_t NotifiesReceived = 0;
		int32_t FooEventsReceived = 0;
		Foo LastFooMessageReceived;
	};

	// Allows custom functions to be called on notify
	struct CustomSubscriber final : public TestSubscriber
	{
		using CustomFunction = std::function<void(const EventBase&)>;

		void SetCustomFunction(const CustomFunction& func)
		{
			_customFunc = func;
		}

		inline void Notify(const EventBase& e) noexcept override
		{
			TestSubscriber::Notify(e);
			if (_customFunc != nullptr)
			{
				_customFunc(e);
			}
		}

	private:
		CustomFunction _customFunc = nullptr;
	};
}

namespace LibraryDesktopTests
{
	

	TEST_CLASS(EventTests)
	{
	public:
		TEST_METHOD_INITIALIZE(Initialize)
		{
#if defined(DEBUG) || defined(_DEBUG)
			_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF);
			_CrtMemCheckpoint(&_startMemState);
#endif
		}

		TEST_METHOD_CLEANUP(Cleanup)
		{
#if defined(DEBUG) || defined(_DEBUG)
			_CrtMemState endMemState, diffMemState;
			_CrtMemCheckpoint(&endMemState);
			if (_CrtMemDifference(&diffMemState, &_startMemState, &endMemState))
			{
				_CrtMemDumpStatistics(&diffMemState);
				Assert::Fail(L"Memory Leaks!");
			}
#endif
		}

		TEST_METHOD(SubscribeUnsubscribe)
		{
			EventPublisher publisher;
			CustomSubscriber subscriber;
			GameTime gameTime;

			// Subscrube
			publisher.Subscribe<Event<Foo>>(subscriber);

			// Sanity check
			publisher.Update(gameTime);
			Assert::AreEqual(0, subscriber.NotifiesReceived);
			Assert::AreEqual(0, subscriber.FooEventsReceived);

			// Enqueue event
			Event<Foo> e(Foo{1});
			Assert::AreEqual(1, e.Message().Data());
			publisher.Enqueue(e, gameTime);

			// Ensure subscribers notified of event
			publisher.Update(gameTime);
			Assert::AreEqual(1, subscriber.NotifiesReceived);
			Assert::AreEqual(1, subscriber.FooEventsReceived);
			Assert::AreEqual(e.Message(), subscriber.LastFooMessageReceived);

			// Ensure event is not sent again
			publisher.Update(gameTime);
			Assert::AreEqual(1, subscriber.NotifiesReceived);
			Assert::AreEqual(1, subscriber.FooEventsReceived);
			Assert::AreEqual(e.Message(), subscriber.LastFooMessageReceived);

			// Enqueue event again
			publisher.Enqueue(e, gameTime);

			// Ensure subscribers notified of event
			publisher.Update(gameTime);
			Assert::AreEqual(2, subscriber.NotifiesReceived);
			Assert::AreEqual(2, subscriber.FooEventsReceived);
			Assert::AreEqual(e.Message(), subscriber.LastFooMessageReceived);

			// Unsubscribe
			publisher.Unsubscribe<Event<Foo>>(subscriber);
			publisher.Enqueue(e, gameTime);

			publisher.Update(gameTime);
			Assert::AreEqual(2, subscriber.NotifiesReceived);
			Assert::AreEqual(2, subscriber.FooEventsReceived);
			Assert::AreEqual(e.Message(), subscriber.LastFooMessageReceived);
		}

		TEST_METHOD(SubscriberUnscribersOnNotify)
		{
			EventPublisher publisher;
			CustomSubscriber subscriber;
			subscriber.SetCustomFunction([&subscriber, &publisher](const EventBase&)
				{

					publisher.Unsubscribe<Event<Foo>>(subscriber);
				});
			GameTime gameTime;

			// Subscrube
			publisher.Subscribe<Event<Foo>>(subscriber);

			// Sanity check
			publisher.Update(gameTime);
			Assert::AreEqual(0, subscriber.NotifiesReceived);
			Assert::AreEqual(0, subscriber.FooEventsReceived);

			// Enqueue event
			Event<Foo> e(Foo{});
			publisher.Enqueue(e, gameTime);

			// Ensure subscribers notified of event
			publisher.Update(gameTime);
			Assert::AreEqual(1, subscriber.NotifiesReceived);
			Assert::AreEqual(1, subscriber.FooEventsReceived);
			Assert::AreEqual(e.Message(), subscriber.LastFooMessageReceived);

			// Enqueue event again
			publisher.Enqueue(e, gameTime);

			// Ensure subscriber not notified of event
			publisher.Update(gameTime);
			Assert::AreEqual(1, subscriber.NotifiesReceived);
			Assert::AreEqual(1, subscriber.FooEventsReceived);
			Assert::AreEqual(e.Message(), subscriber.LastFooMessageReceived);
		}

		TEST_METHOD(SubscriberSubscribesAgainOnNotify)
		{
			EventPublisher publisher;
			CustomSubscriber subscriber;
			subscriber.SetCustomFunction([&subscriber, &publisher](const EventBase&)
				{
					publisher.Subscribe<Event<Foo>>(subscriber);
				});
			GameTime gameTime;

			// Subscrube
			publisher.Subscribe<Event<Foo>>(subscriber);

			// Sanity check
			publisher.Update(gameTime);
			Assert::AreEqual(0, subscriber.NotifiesReceived);
			Assert::AreEqual(0, subscriber.FooEventsReceived);

			// Enqueue event
			Event<Foo> e(Foo{});
			publisher.Enqueue(e, gameTime);

			// Ensure subscribers notified of event
			publisher.Update(gameTime);
			Assert::AreEqual(1, subscriber.NotifiesReceived);
			Assert::AreEqual(1, subscriber.FooEventsReceived);
			Assert::AreEqual(e.Message(), subscriber.LastFooMessageReceived);

			// Enqueue event again
			publisher.Enqueue(e, gameTime);

			// Ensure subscriber notified of event again but twice
			publisher.Update(gameTime);
			Assert::AreEqual(3, subscriber.NotifiesReceived);
			Assert::AreEqual(3, subscriber.FooEventsReceived);
			Assert::AreEqual(e.Message(), subscriber.LastFooMessageReceived);
		}

		TEST_METHOD(SubscriberEnqueuesOnNotify)
		{
			EventPublisher publisher;
			CustomSubscriber subscriber;
			GameTime gameTime;
			subscriber.SetCustomFunction([&publisher, &gameTime](const EventBase& e)
				{
					publisher.Enqueue(e, gameTime);
				});

			// Subscrube
			publisher.Subscribe<Event<Foo>>(subscriber);

			// Sanity check
			publisher.Update(gameTime);
			Assert::AreEqual(0, subscriber.NotifiesReceived);
			Assert::AreEqual(0, subscriber.FooEventsReceived);

			// Enqueue 1 events
			Event<Foo> e(Foo{});
			publisher.Enqueue(e, gameTime);

			// Check size of queue
			Assert::AreEqual(1_z, publisher.Size());

			// Ensure subscribers notified of 1 events
			publisher.Update(gameTime);
			Assert::AreEqual(1, subscriber.NotifiesReceived);
			Assert::AreEqual(1, subscriber.FooEventsReceived);
			Assert::AreEqual(e.Message(), subscriber.LastFooMessageReceived);

			// Check size of queue
			Assert::AreEqual(1_z, publisher.Size());

			// Ensure subscriber gets another event
			publisher.Update(gameTime);
			Assert::AreEqual(2, subscriber.NotifiesReceived);
			Assert::AreEqual(2, subscriber.FooEventsReceived);
			Assert::AreEqual(e.Message(), subscriber.LastFooMessageReceived);

			Assert::AreEqual(1_z, publisher.Size());
		}

		TEST_METHOD(SubscriberClearsOnNotify)
		{
			EventPublisher publisher;
			CustomSubscriber subscriber;
			subscriber.SetCustomFunction([&publisher](const EventBase&)
				{
					publisher.Clear();
				});
			GameTime gameTime;

			// Subscrube
			publisher.Subscribe<Event<Foo>>(subscriber);

			// Sanity check
			publisher.Update(gameTime);
			Assert::AreEqual(0, subscriber.NotifiesReceived);
			Assert::AreEqual(0, subscriber.FooEventsReceived);

			// Enqueue 3 events
			Event<Foo> e(Foo{});
			Event<Foo> e2(Foo{});
			publisher.Enqueue(e, gameTime);
			publisher.Enqueue(e, gameTime);
			publisher.Enqueue(e, gameTime, 1ms);

			// Check size of queue
			Assert::AreEqual(3_z, publisher.Size());

			// Ensure subscribers notified of 2 events
			publisher.Update(gameTime);
			Assert::AreEqual(2, subscriber.NotifiesReceived);
			Assert::AreEqual(2, subscriber.FooEventsReceived);
			Assert::AreEqual(e.Message(), subscriber.LastFooMessageReceived);

			// Check size of queue
			Assert::AreEqual(0_z, publisher.Size());

			// Increase time by 1ms
			// Ensure subscriber not notified
			gameTime.SetCurrentTime(gameTime.CurrentTime() + 2ms);
			publisher.Update(gameTime);
			Assert::AreEqual(2, subscriber.NotifiesReceived);
			Assert::AreEqual(2, subscriber.FooEventsReceived);
			Assert::AreEqual(e.Message(), subscriber.LastFooMessageReceived);
		}

		TEST_METHOD(QueueSize)
		{
			EventPublisher publisher;
			GameTime gameTime;

			Assert::IsTrue(publisher.IsEmpty());
			Assert::AreEqual(0_z, publisher.Size());

			publisher.Enqueue(Event<Foo>{Foo{}}, gameTime);
			Assert::IsFalse(publisher.IsEmpty());
			Assert::AreEqual(1_z, publisher.Size());
			
			publisher.Enqueue(Event<Foo>{Foo{}}, gameTime);
			Assert::IsFalse(publisher.IsEmpty());
			Assert::AreEqual(2_z, publisher.Size());

			publisher.Clear();
			Assert::IsTrue(publisher.IsEmpty());
			Assert::AreEqual(0_z, publisher.Size());
		}

	private:
		inline static _CrtMemState _startMemState;
	};
}