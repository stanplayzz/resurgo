#pragma once
#include "resurgo/gameplay/chunk.hpp"
#include "resurgo/gameplay/tile.hpp"
#include "resurgo/utils/perlin_noise.hpp"
#include <clib/not_null.hpp>

namespace resurgo {
// Used to efficiently look up chunks
struct ChunkHash {
	std::size_t operator()(sf::Vector2i const& p) const noexcept {
		auto h1 = std::hash<int>{}(p.x);
		auto h2 = std::hash<int>{}(p.y);
		return h1 ^ (h2 << 1);
	}
};

class ChunkLoader {
  public:
	void update(sf::View const& view);
	void draw(sf::RenderTarget& target) const;

  private:
	auto getTileAt(sf::Vector2f coords) const -> Tile;

	void loadChunks(sf::Vector2i centerChunk);
	void unloadChunks(sf::Vector2i centerChunk);

	std::unordered_map<sf::Vector2i, Chunk, ChunkHash> m_chunks{};
	sf::Vector2i m_lastCenterChunk{};

	siv::PerlinNoise m_noise{12345u};
};
} // namespace resurgo