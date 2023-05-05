#include "GameObject.h"

namespace FieaGameEngine
{
	template<std::derived_from<Attributed> T>
	GameObject::GameObject(T* t) : Attributed(t)
	{
	}

	template<std::derived_from<Attributed> T>
	GameObject::GameObject(const GameObject& rhs, T* t) : 
		Name(rhs.Name),
		Transform(rhs.Transform),
		Attributed(rhs, t)
	{
	}

	template<std::derived_from<Attributed> T>
	GameObject::GameObject(GameObject&& rhs, T* t) noexcept : 
		Name(std::move(rhs.Name)),
		Transform(std::move(rhs.Transform)),
		Attributed(std::move(rhs), t)
	{
	}
}