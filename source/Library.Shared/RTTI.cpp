#include "pch.h"
#include "RTTI.h"

namespace FieaGameEngine
{
	bool RTTI::operator==(const RTTI& rhs) const
	{
		return Equals(&rhs);
	}

	bool RTTI::operator!=(const RTTI& rhs) const
	{
		return !Equals(&rhs);
	}

	std::string RTTI::ToString() const
	{
		using namespace std::string_literals;
		return "RTTI"s;
	}
	
	bool RTTI::Equals(const RTTI*) const
	{
		return false;
	}
}