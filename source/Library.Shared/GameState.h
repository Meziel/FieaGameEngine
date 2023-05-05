#pragma once

#include "GameClock.h"
#include "GameTime.h"

namespace FieaGameEngine
{
	class GameState
	{
	public:
		GameClock GameClock;
		GameTime GameTime;
	};
}