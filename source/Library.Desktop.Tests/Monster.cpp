#include "pch.h"

#include "Monster.h"

using namespace FieaGameEngine;

namespace UnitTests
{
	ATTRIBUTED_DEFINITIONS(Monster, GameObject);

	Monster::Monster() : GameObject(this)
	{
	}

	Monster::Monster(const Monster& rhs) :
		Counter(rhs.Counter),
		GameObject(rhs, this)
	{
	}

	Monster::Monster(Monster&& rhs) noexcept :
		Counter(rhs.Counter),
		GameObject(std::move(rhs), this)
	{
	}

	void Monster::Update(const FieaGameEngine::GameTime& gameTime)
	{
		++Counter;
		GameObject::Update(gameTime);
	}

	std::unique_ptr<Scope> Monster::Clone() const
	{
		return std::make_unique<Monster>(*this);
	}

	FieaGameEngine::Vector<FieaGameEngine::Signature> Monster::Signatures()
	{
		return {};
	}
}