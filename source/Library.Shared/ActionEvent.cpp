#include "pch.h"
#include "ActionEvent.h"
#include "Game.h"
#include "EventMessageAttributed.h"

using namespace std;

namespace FieaGameEngine
{
	ATTRIBUTED_DEFINITIONS(ActionEvent, Action);

	ActionEvent::ActionEvent() : Action(this)
	{
	}

	ActionEvent::ActionEvent(const ActionEvent& rhs) :
		Subtype(rhs.Subtype),
		Delay(rhs.Delay),
		Action(rhs, this)
	{
	}

	ActionEvent::ActionEvent(ActionEvent&& rhs) noexcept :
		Subtype(rhs.Subtype),
		Delay(rhs.Delay),
		Action(std::move(rhs), this)
	{
	}

	void ActionEvent::Update(const GameTime& time)
	{
		EventMessageAttributed message;
		message.Subtype = Subtype;

		// Copy aux attributes to message
		ForEachAuxiliaryAttribute(
			[&message](const std::pair<const std::string, Datum>* datumPair)
			{
				Datum& attribute = message.AppendAuxilaryAttribute(datumPair->first);
				attribute = datumPair->second;
				return false;
			}
		);

		Game::Instance().Publisher.Enqueue(Event{std::move(message)}, time, chrono::milliseconds(Delay));
	}

	std::unique_ptr<Scope> ActionEvent::Clone() const
	{
		return make_unique<ActionEvent>(*this);
	}

	Vector<Signature> ActionEvent::Signatures()
	{
		return
		{
			{"Subtype", DatumTypes::Integer, true, Signature::ExternalInfo{1, offsetof(ActionEvent, Subtype)}},
			{"Delay", DatumTypes::Integer, true, Signature::ExternalInfo{1, offsetof(ActionEvent, Delay)}}
		};
	}
}