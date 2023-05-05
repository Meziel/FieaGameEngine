#pragma once

#include <memory>
#include "EventPublisher.h"
#include "GameState.h"
#include "GameObject.h"
#include "EventPublisher.h"

namespace FieaGameEngine
{
	class Game final
	{
	public:
		Game& operator=(const Game& rhs) = delete;
		Game& operator=(Game&& rhs) = delete;
		~Game() = default;

		/// <summary>
		/// Gets the instance of the game
		/// </summary>
		/// <returns></returns>
		static Game& Instance();

		/// <summary>
		/// Clears the memory for the game instance
		/// </summary>
		static void Reset();

		/// <summary>
		/// Updates the game by one tick
		/// </summary>
		void Update();

		/// <summary>
		/// Event publisher
		/// </summary>
		EventPublisher Publisher;

		/// <summary>
		/// State of the game
		/// </summary>
		GameState GameState;

		/// <summary>
		/// Root game object
		/// </summary>
		std::unique_ptr<GameObject> Root;
	protected:
		Game() = default;

	private:
		inline static std::unique_ptr<Game> _instance;
	};
}
