#include "DefaultReserveStrategy.h"

namespace FieaGameEngine
{
	inline size_t DefaultReserveStrategy::operator()(std::size_t capacity)
	{
		return capacity == 0 ? 1 : capacity * 2;
	}
}