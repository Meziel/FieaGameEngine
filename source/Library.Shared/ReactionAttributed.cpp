#include "pch.h"
#include "ReactionAttributed.h"
#include "EventMessageAttributed.h"
#include "Game.h"

using namespace std;

namespace FieaGameEngine
{
	ATTRIBUTED_DEFINITIONS(ReactionAttributed, Reaction);

	ReactionAttributed::ReactionAttributed() : Reaction(this)
	{
		Game::Instance().Publisher.Subscribe<Event<EventMessageAttributed>>(*this);
	}

	ReactionAttributed::ReactionAttributed(const ReactionAttributed& rhs) :
		Subtype(rhs.Subtype),
		Reaction(rhs, this)
	{
		Game::Instance().Publisher.Subscribe<Event<EventMessageAttributed>>(*this);
	}

	ReactionAttributed::ReactionAttributed(ReactionAttributed&& rhs) noexcept :
		Subtype(rhs.Subtype),
		Reaction(std::move(rhs), this)
	{
		Game::Instance().Publisher.Subscribe<Event<EventMessageAttributed>>(*this);
	}

	ReactionAttributed::~ReactionAttributed()
	{
		Game::Instance().Publisher.Unsubscribe<Event<EventMessageAttributed>>(*this);
	}

	void ReactionAttributed::Notify(const EventBase& e) noexcept
	{
		assert(e.Is(Event<EventMessageAttributed>::TypeIdClass()));
		const Event<EventMessageAttributed>& messageAttributedEvent = static_cast<const Event<EventMessageAttributed>&>(e);
		const EventMessageAttributed& message = messageAttributedEvent.Message();
		if (message.Subtype == Subtype)
		{
			// Copy event aux attributes to child actions
			Datum& actions = Actions();
			for (size_t i = 0; i < actions.Size(); ++i)
			{
				Scope& actionScope = *actions.Get<shared_ptr<Scope>>(i);
				assert(actionScope.Is(Action::TypeIdClass()));
				Action& action = static_cast<Action&>(actionScope);
				message.ForEachAuxiliaryAttribute(
					[&message, &action](const std::pair<const std::string, Datum>* datumPair)
					{
						// TODO: cannot support prescribed attributes
						Datum& attribute = action.Append(datumPair->first);
						attribute = datumPair->second;
						// In case we overrode external scopes, we need to fix pointers
						action.Populate(action.AllSignatures());
						return false;
					}
				);
			}

			ActionList::Update(Game::Instance().GameState.GameTime);
		}
	}

	std::unique_ptr<Scope> ReactionAttributed::Clone() const
	{
		return make_unique<ReactionAttributed>();
	}

	Vector<Signature> ReactionAttributed::Signatures()
	{
		return
		{
			{"Subtype", DatumTypes::Integer, true, Signature::ExternalInfo{1, offsetof(ReactionAttributed, Subtype)}}
		};
	}
}