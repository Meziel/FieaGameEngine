#include "pch.h"
#include "Datum.h"
#include "Scope.h"
#include <variant>

using namespace std;

namespace FieaGameEngine
{
	inline Scope& Datum::operator[](std::size_t index)
	{
		return *Get<shared_ptr<Scope>>(index);
	}
}