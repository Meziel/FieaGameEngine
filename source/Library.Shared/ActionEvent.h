#pragma once

#include "Action.h"

namespace FieaGameEngine
{
	class ActionEvent final : public Action
	{
		ATTRIBUTED_DECLARATIONS(ActionEvent, Action);

	public:
		ActionEvent();
		ActionEvent(const ActionEvent& rhs);
		ActionEvent(ActionEvent&& rhs) noexcept;
		ActionEvent& operator=(const ActionEvent& rhs) = default;
		ActionEvent& operator=(ActionEvent&& rhs) noexcept = default;

		/// <summary>
		/// Subtype of the event message attributed
		/// </summary>
		int32_t Subtype;

		/// <summary>
		/// Delay of the event
		/// </summary>
		int32_t Delay;
    
		/// <summary>
		/// Game loop called every tick
		/// </summary>
		/// <param name="time"></param>
		void Update(const GameTime& time) override;

		/// <summary>
		/// Polymorphicly creates a copy of the object
		/// </summary>
		/// <returns></returns>
		[[nodiscard]] std::unique_ptr<Scope> Clone() const override;

		/// <summary>
		/// Lists of prescribed attributes for this class
		/// </summary>
		/// <returns></returns>
		[[nodiscard]] static Vector<Signature> Signatures();
	};
	
	ConcreteFactory(ActionEvent, Scope);
}