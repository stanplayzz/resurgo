#pragma once
#include "resurgo/resources.hpp"
#include <cstddef>

namespace resurgo {
constexpr auto tileTextureOffset_v = 512;
constexpr auto tileSize_v = 128;

enum class TileId : std::int8_t {
	Soil = 0,
	DarkSoil = 1,
};

struct Tile {
	TileId id{};
	sf::Vector3i position{};
	int leftFaceHeight{};
	int rightFaceHeight{};
};

inline auto getTileRectFromId(std::size_t id) {
	auto const& tileset = *Resources::instance().load<sf::Texture>("images/tileset.png");
	auto texuresPerRow = tileset.getSize().x / tileTextureOffset_v;
	auto row = id / texuresPerRow;
	auto col = id % texuresPerRow;

	return sf::FloatRect{{static_cast<float>(col * tileTextureOffset_v), static_cast<float>(row * tileTextureOffset_v)},
						 {tileTextureOffset_v, tileTextureOffset_v}};
}
} // namespace resurgo