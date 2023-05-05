#pragma once

#include "Reaction.h"

namespace FieaGameEngine
{
	class ReactionAttributed final : public Reaction
	{
		ATTRIBUTED_DECLARATIONS(ReactionAttributed, Reaction);

	public:
		ReactionAttributed();
		ReactionAttributed(const ReactionAttributed& rhs);
		ReactionAttributed(ReactionAttributed&& rhs) noexcept;
		ReactionAttributed& operator=(const ReactionAttributed& rhs) = default;
		ReactionAttributed& operator=(ReactionAttributed&& rhs) noexcept = default;
		~ReactionAttributed();

		/// <summary>
		/// Subtype to match for event message
		/// </summary>
		int32_t Subtype = 0;

		/// <summary>
		/// Callback method for receiving events
		/// </summary>
		/// <param name="e">Event received</param>
		void Notify(const EventBase& e) noexcept override;

		/// <summary>
		/// Polymorphicly creates a copy of the ActionList
		/// </summary>
		/// <returns></returns>
		[[nodiscard]] std::unique_ptr<Scope> Clone() const override;

		/// <summary>
		/// Lists of prescribed attributes for this class
		/// </summary>
		/// <returns></returns>
		[[nodiscard]] static Vector<Signature> Signatures();
	};

	ConcreteFactory(ReactionAttributed, Scope);
}