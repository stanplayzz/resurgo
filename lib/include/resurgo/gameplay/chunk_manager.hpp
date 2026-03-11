#pragma once
#include "resurgo/gameplay/chunk.hpp"
#include "resurgo/gameplay/settings.hpp"
#include "resurgo/utils/perlin_noise.hpp"
#include <clib/not_null.hpp>

namespace resurgo {
// Used to efficiently look up chunks
struct ChunkHash {
	std::size_t operator()(sf::Vector2i const& p) const noexcept {
		return std::hash<std::int64_t>{}((static_cast<std::int64_t>(p.x) << 32) | static_cast<std::uint32_t>(p.y));
	}
};

class ChunkManager {
  public:
	void update(sf::View const& view);
	void draw(sf::RenderTarget& target, Settings const& settings) const;

	[[nodiscard]] auto getChunkAt(sf::Vector2i coord) const -> Chunk const* {
		auto it = m_chunks.find(coord);
		return it != m_chunks.end() ? &it->second : nullptr;
	}

	[[nodiscard]] auto allChunks() const -> std::unordered_map<sf::Vector2i, Chunk, ChunkHash> const& {
		return m_chunks;
	}

  private:
	void loadChunks(sf::Vector2i centerChunk);
	void unloadChunks(sf::Vector2i centerChunk);
	void computeChunkFaces();

	std::unordered_map<sf::Vector2i, Chunk, ChunkHash> m_chunks{};
	sf::Vector2i m_lastCenterChunk{};

	siv::PerlinNoise m_noise{12345u};
};
} // namespace resurgo