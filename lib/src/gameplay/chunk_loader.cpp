#include "resurgo/gameplay/chunk_loader.hpp"
#include <cmath>
#include <print>

namespace resurgo {
namespace {
constexpr auto renderDistance_v = 5;

constexpr auto noiseScale_v = 0.02f;
constexpr auto noiseOctaves_v = 3;
} // namespace

void ChunkLoader::update(sf::View const& view) {
	auto centerChunk = sf::Vector2i{static_cast<int>(std::floor(view.getCenter().x / (chunkSize_v * tileSize_v))),
									static_cast<int>(std::floor(view.getCenter().y / (chunkSize_v * tileSize_v)))};

	if (centerChunk != m_lastCenterChunk) {
		m_lastCenterChunk = centerChunk;
		loadChunks(centerChunk);
		unloadChunks(centerChunk);
	}

	std::println("{}", m_chunks.size());
}

auto ChunkLoader::getTileAt(sf::Vector2f coords) const -> Tile {
	auto scaled = coords * noiseScale_v;
	auto noise = m_noise.octave2D_01(scaled.x, scaled.y, noiseOctaves_v);

	auto tile = Tile{};
	if (noise < 0.3f) { tile = Tile::LowSoil; }
	if (noise > 0.7f) { tile = Tile::Rock; }
	return tile;
}

void ChunkLoader::loadChunks(sf::Vector2i centerChunk) {
	for (auto y = -renderDistance_v; y <= renderDistance_v; y++) {
		for (auto x = -renderDistance_v; x <= renderDistance_v; x++) {
			auto coord = sf::Vector2i{centerChunk.x + x, centerChunk.y + y};

			if (!m_chunks.contains(coord)) {
				auto chunk = Chunk{};
				auto data = ChunkData{};
				auto layer = Layer{};

				for (auto i = 0; i < chunkSize_v * chunkSize_v; i++) {
					auto tilePosition = sf::Vector2i{(coord.x * chunkSize_v) + (i % chunkSize_v),
													 (coord.y * chunkSize_v) + (i / chunkSize_v)};
					layer.tiles.at(static_cast<std::size_t>(i)) = getTileAt({tilePosition});
				}

				data.layers.push_back(layer);

				chunk.generate(data);
				chunk.setPosition(coord);

				m_chunks.emplace(coord, std::move(chunk));
			}
		}
	}
}

void ChunkLoader::unloadChunks(sf::Vector2i centerChunk) {
	for (auto it = m_chunks.begin(); it != m_chunks.end();) {
		auto dx = std::abs(it->first.x - centerChunk.x);
		auto dy = std::abs(it->first.y - centerChunk.y);

		if (dx > renderDistance_v || dy > renderDistance_v) {
			it = m_chunks.erase(it);
		} else {
			it++;
		}
	}
}

void ChunkLoader::draw(sf::RenderTarget& target) const {
	for (auto const& chunk : m_chunks) { chunk.second.draw(target); }
}
} // namespace resurgo