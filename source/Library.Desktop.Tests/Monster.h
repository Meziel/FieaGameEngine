#pragma once

#include "GameObject.h"

namespace UnitTests
{
	class Monster final : public FieaGameEngine::GameObject
	{
		ATTRIBUTED_DECLARATIONS(Monster, FieaGameEngine::GameObject)

	public:
		Monster();
		Monster(const Monster& rhs);
		Monster(Monster&& rhs) noexcept;
		Monster& operator=(const Monster& rhs) = default;
		Monster& operator=(Monster&& rhs) = default;

		size_t Counter = 0;

		void Update(const FieaGameEngine::GameTime& gameTime) override;

		[[nodiscard]] std::unique_ptr<Scope> Clone() const override; 
		[[nodiscard]] static FieaGameEngine::Vector<FieaGameEngine::Signature> Signatures();
	};

	ConcreteFactory(Monster, FieaGameEngine::Scope);
}
