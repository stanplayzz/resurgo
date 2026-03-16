#pragma once
#include "resurgo/gameplay/chunk.hpp"
#include "resurgo/gameplay/chunk_manager.hpp"
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

constexpr auto getTileFromPoint(sf::Vector2f point, ChunkManager const& chunks) -> Tile const* {
	point.x -= tileSize_v * 0.5f;

	auto tilePos = IsoTransform::screenToTile(point);
	auto localPos = IsoTransform::tileToLocalCoord(tilePos);
	auto chunkPos = IsoTransform::tileToChunk(tilePos);

	auto const* chunk = chunks.getChunkAt(chunkPos);
	if (!chunk) { return nullptr; }

	auto const* tile = chunk->tileAt(localPos);
	if (!tile) { return nullptr; }

	point.y += static_cast<float>(tile->position.z * tileSize_v) * 0.25f;
	tilePos = IsoTransform::screenToTile(point);
	localPos = IsoTransform::tileToLocalCoord(tilePos);
	chunkPos = IsoTransform::tileToChunk(tilePos);

	chunk = chunks.getChunkAt(chunkPos);
	if (!chunk) { return nullptr; }

	return chunk->tileAt(localPos);
}

} // namespace resurgo::IsoTransform