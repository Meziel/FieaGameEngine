#pragma once

#include "Attributed.h"

namespace FieaGameEngine
{
	class EventMessageAttributed : public Attributed
	{
		ATTRIBUTED_DECLARATIONS(EventMessageAttributed, Attributed);
	
	public:
		EventMessageAttributed();
		EventMessageAttributed(const EventMessageAttributed& rhs);
		EventMessageAttributed(EventMessageAttributed&& rhs) noexcept;
		EventMessageAttributed& operator=(const EventMessageAttributed& rhs) = default;
		EventMessageAttributed& operator=(EventMessageAttributed&& rhs) noexcept = default;

		/// <summary>
		/// Polymorphicly creates a copy of the GameObject
		/// </summary>
		/// <returns></returns>
		[[nodiscard]] std::unique_ptr<Scope> Clone() const override; 

		/// <summary>
		/// Lists of prescribed attributes
		/// </summary>
		/// <returns></returns>
		[[nodiscard]] static Vector<Signature> Signatures();

		/// <summary>
		/// Subtype of the event
		/// </summary>
		int32_t Subtype;
	};

	ConcreteFactory(EventMessageAttributed, Scope);
}
