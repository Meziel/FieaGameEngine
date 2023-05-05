#pragma once

#include <exception>
#include <chrono>

namespace FieaGameEngine
{
	class GameTime;

	class GameClock final
	{
	public:
		GameClock();
		GameClock(const GameClock&) = default;
		GameClock& operator=(const GameClock&) = default;
		GameClock(GameClock&&) = default;
		GameClock& operator=(GameClock&&) = default;
		~GameClock() = default;

		[[nodiscard]] const std::chrono::high_resolution_clock::time_point& StartTime() const;
		[[nodiscard]] const std::chrono::high_resolution_clock::time_point& CurrentTime() const;
		[[nodiscard]] const std::chrono::high_resolution_clock::time_point& LastTime() const;

		void Reset();
		void UpdateGameTime(GameTime& gameTime);

	private:
		std::chrono::high_resolution_clock::time_point _startTime;
		std::chrono::high_resolution_clock::time_point _currentTime;
		std::chrono::high_resolution_clock::time_point _lastTime;
	};
}