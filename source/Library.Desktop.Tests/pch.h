#pragma once

#include <cstdint>
#include <stdexcept>
#include <cstddef>
#include <stdlib.h>
#include <initializer_list>
#include <utility>
#include <cassert>
#include <algorithm>
#include <functional>
#include <memory>
#include <memory_resource>
#include <glm/glm.hpp>
#include <type_traits>
#include <sstream>
#include <glm/glm.hpp>
#pragma warning(push)
#pragma warning(disable:4201)
#include <glm/gtx/string_cast.hpp>
#pragma warning(pop)

#ifndef UNREFERENCED_LOCAL
#define UNREFERENCED_LOCAL(L) (L)
#endif // !UNREFERENCED_LOCAL