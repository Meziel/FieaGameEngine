#pragma once

#include "DefaultHash.h"
#include "Foo.h"

namespace FieaGameEngine
{
	template <>
	struct DefaultHash<UnitTests::Foo>
	{
		size_t operator()(const UnitTests::Foo& key)
		{
			return static_cast<size_t>(key.Data());
		}
	};
}