#pragma once

#include <cstddef>

namespace FieaGameEngine
{
	/// <summary>
	/// Default functor for increasing capcity
	/// </summary>
	struct DefaultReserveStrategy final
	{
		/// <summary>
		/// Calculates a new capacity given the current capacity
		/// </summary>
		/// <param name="capacity">Current capacity</param>
		/// <returns>New capacity</returns>
		inline size_t operator()(std::size_t capacity);
	};
}

#include "DefaultReserveStrategy.inl"