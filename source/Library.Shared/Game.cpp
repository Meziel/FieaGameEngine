#include "pch.h"

#include "Game.h"

namespace FieaGameEngine
{
	Game& Game::Instance()
	{
		if (_instance == nullptr)
		{
			_instance = std::unique_ptr<Game>(new Game());
		}
		return *_instance;
	}

	void Game::Reset()
	{
		_instance.get_deleter()(_instance.get());
		_instance.release();
	}

	void Game::Update()
	{
		Publisher.Update(GameState.GameTime);

		Root->Update(GameState.GameTime);
	}
}