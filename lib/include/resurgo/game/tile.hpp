#pragma once
#include "resurgo/engine/transform.hpp"
#include <glm/vec3.hpp>

namespace resurgo {
constexpr auto tileSize_v = 64;
constexpr auto pixelsPerTile_v = 8;

struct Tile {
	engine::Transform transform{};
};
} // namespace resurgo