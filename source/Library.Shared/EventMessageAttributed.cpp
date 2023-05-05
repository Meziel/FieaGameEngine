#include "pch.h"

#include "EventMessageAttributed.h"

using namespace std;

namespace FieaGameEngine
{
	ATTRIBUTED_DEFINITIONS(EventMessageAttributed, Attributed);

	EventMessageAttributed::EventMessageAttributed() :
		Attributed(this)
	{
	}

	EventMessageAttributed::EventMessageAttributed(const EventMessageAttributed& rhs) :
		Subtype(rhs.Subtype),
		Attributed(rhs, this)
	{
	}

	EventMessageAttributed::EventMessageAttributed(EventMessageAttributed&& rhs) noexcept : 
		Subtype(rhs.Subtype),
		Attributed(std::move(rhs), this)
	{
	}

	std::unique_ptr<Scope> EventMessageAttributed::Clone() const
	{
		return make_unique<EventMessageAttributed>();
	}

	Vector<Signature> EventMessageAttributed::Signatures()
	{
		return {};
	}
}