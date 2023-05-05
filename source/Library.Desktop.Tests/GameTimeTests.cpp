#include "pch.h"
#include "CppUnitTest.h"
#include "GameTime.h"
#include "GameClock.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;
using namespace glm;
using namespace std;
using namespace std::chrono;
using namespace std::chrono_literals;

namespace Microsoft::VisualStudio::CppUnitTestFramework
{
	template<>
	inline wstring ToString<duration<float>>(const duration<float>& t)
	{
		RETURN_WIDE_STRING(t.count());
	}

	template<>
	inline wstring ToString<milliseconds>(const milliseconds& t)
	{
		RETURN_WIDE_STRING(t.count());
	}

	template<>
	inline wstring ToString<high_resolution_clock::time_point>(const high_resolution_clock::time_point& t)
	{
		RETURN_WIDE_STRING(t.time_since_epoch().count());
	}
}

namespace LibraryDesktopTests
{

	TEST_CLASS(GameTimeTests)
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

		TEST_METHOD(Constructor)
		{
			const GameTime gameTime;

			Assert::AreEqual(0ms, gameTime.TotalGameTime());
			Assert::AreEqual(0ms, gameTime.ElapsedGameTime());
			Assert::AreEqual(0.0f, gameTime.TotalGameTimeSeconds().count());
			Assert::AreEqual(0.0f, gameTime.ElapsedGameTimeSeconds().count());
		}

		TEST_METHOD(GetSetCurrentTime)
		{
			GameTime gameTime;
			Assert::AreEqual(high_resolution_clock::time_point(), gameTime.CurrentTime());

			auto now = high_resolution_clock::now();
			gameTime.SetCurrentTime(now);
			Assert::AreEqual(now, gameTime.CurrentTime());
		}

		TEST_METHOD(GetSetTotalGameTime)
		{
			GameTime gameTime;
			Assert::AreEqual(0ms, gameTime.TotalGameTime());
			Assert::AreEqual(0.0f, gameTime.TotalGameTimeSeconds().count());

			gameTime.SetTotalGameTime(1s);
			Assert::AreEqual(1000ms, gameTime.TotalGameTime());
			Assert::AreEqual(1.0f, gameTime.TotalGameTimeSeconds().count());
		}

		TEST_METHOD(GetSetTotalElapsedTime)
		{
			GameTime gameTime;
			Assert::AreEqual(0ms, gameTime.ElapsedGameTime());
			Assert::AreEqual(0.0f, gameTime.ElapsedGameTimeSeconds().count());
			
			gameTime.SetElapsedGameTime(1s);
			Assert::AreEqual(1000ms, gameTime.ElapsedGameTime());
			Assert::AreEqual(1.0f, gameTime.ElapsedGameTimeSeconds().count());
		}

		TEST_METHOD(GameClockConstructor)
		{
			GameClock clock;
			Assert::AreEqual(clock.CurrentTime(), clock.StartTime());
			Assert::AreEqual(clock.CurrentTime(), clock.LastTime());
		}

		TEST_METHOD(UpdateGameTime)
		{
			GameClock clock;
			GameTime gameTime;

			const auto& lastTime = clock.LastTime();
			clock.UpdateGameTime(gameTime);
			Assert::AreEqual(clock.CurrentTime(), gameTime.CurrentTime());
			Assert::AreEqual(duration_cast<milliseconds>(clock.CurrentTime() - clock.StartTime()), gameTime.TotalGameTime());
			Assert::AreEqual(duration_cast<milliseconds>(clock.CurrentTime() - lastTime), gameTime.ElapsedGameTime());
		}

	private:
		inline static _CrtMemState _startMemState;
	};
}