#pragma once
#include "resurgo/resources.hpp"
#include <cstddef>

namespace resurgo {
constexpr auto tileTextureOffset_v = 16;
constexpr auto tileSize_v = 32;

enum class Tile : std::int8_t {
	Soil = 0,
	Rock = 1,
	LowSoil = 2,
};

constexpr auto getTileRectFromId(std::size_t id) {
	auto const& tileset = *Resources::instance().load<sf::Texture>("images/tileset.png");
	auto texuresPerRow = tileset.getSize().x / tileTextureOffset_v;
	auto row = id % texuresPerRow;
	auto col = id / texuresPerRow;

	return sf::FloatRect{{static_cast<float>(row * tileTextureOffset_v), static_cast<float>(col * tileTextureOffset_v)},
						 {tileTextureOffset_v, tileTextureOffset_v}};
}
} // namespace resurgo