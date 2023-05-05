#pragma once

#include <string>
#include "Datum.h"
#include <Variant>

namespace FieaGameEngine
{
	struct Signature
	{
		struct ExternalInfo
		{
			std::size_t Size;
			std::size_t Offset;
		};

		std::string Name;
		DatumTypes Type;
		bool IsExternal;
		std::variant<std::monostate, ExternalInfo> SigInfo;
	};
}