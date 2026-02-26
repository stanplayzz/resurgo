#pragma once
#include "clib/enum_array.hpp"
#include <cstdint>
#include <string_view>

namespace resurgo {
enum class Item : std::uint8_t {
	Wood,
	Iron,
	Copper,
	Aluminum,
	COUNT_,
};

constexpr auto itemName_v = clib::EnumArray<Item, std::string_view>{
	"Wood",
	"Iron",
	"Copper",
	"Aluminum",
};
} // namespace resurgo