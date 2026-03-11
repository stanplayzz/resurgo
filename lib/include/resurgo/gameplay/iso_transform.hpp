#pragma once
#include "resurgo/gameplay/chunk.hpp"
#include <SFML/System/Vector2.hpp>

namespace resurgo::IsoTransform {

namespace {
constexpr auto floorDiv(int a, int b) -> int { return (a / b) - (a % b != 0 && (a ^ b) < 0); }
} // namespace

constexpr auto screenToTile(sf::Vector2f screenPos) -> sf::Vector2i {
	return {
		static_cast<int>(std::floor((screenPos.x / (tileSize_v * 0.5f) + screenPos.y / (tileSize_v * 0.25f)) / 2.f)),
		static_cast<int>(std::floor((screenPos.y / (tileSize_v * 0.25f) - screenPos.x / (tileSize_v * 0.5f)) / 2.f))};
}

constexpr auto tileToScreen(sf::Vector2i tilePos, int z = 0) -> sf::Vector2f {
	return {(static_cast<float>(tilePos.x - tilePos.y) * (tileSize_v * 0.5f)),
			(static_cast<float>(tilePos.x + tilePos.y) * (tileSize_v * 0.25f)) -
				(static_cast<float>(z) * (tileSize_v * 0.25f))};
}

constexpr auto tileToScreen(sf::Vector3i tilePos) -> sf::Vector2f {
	return tileToScreen({tilePos.x, tilePos.y}, tilePos.z);
}

constexpr auto tileToChunk(sf::Vector2i tilePos) -> sf::Vector2i {
	return {floorDiv(tilePos.x, chunkSize_v), floorDiv(tilePos.y, chunkSize_v)};
}

constexpr auto tileToLocalCoord(sf::Vector2i tilePos) -> sf::Vector2i {
	return {((tilePos.x % chunkSize_v) + chunkSize_v) % chunkSize_v,
			((tilePos.y % chunkSize_v) + chunkSize_v) % chunkSize_v};
}

} // namespace resurgo::IsoTransform