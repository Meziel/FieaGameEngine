#pragma once
#include <cstddef>
#include <cstdint>

constexpr std::size_t operator "" _z(std::uint64_t n)
{
    return static_cast<std::size_t>(n);
}
