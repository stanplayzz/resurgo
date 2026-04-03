#pragma once
#include "resurgo/engine/renderer.hpp"
#include "resurgo/game/chunk.hpp"
#include <glm/vec2.hpp>

namespace resurgo {
struct ChunkHash {
	auto operator()(glm::ivec2 const& vec) const noexcept -> std::size_t {
		std::hash<int> hasher;
		auto h1 = hasher(vec.x);
		auto h2 = hasher(vec.y);
		return h1 ^ (h2 * 2654435761u);
	};
};

class ChunkManager {
  public:
	void update(glm::vec3 playerPos);
	void draw(engine::Renderer& renderer) const;

	[[nodiscard]] auto getChunkAt(glm::ivec2 coords) const -> Chunk const* {
		auto it = m_chunks.find(coords);
		if (it == m_chunks.end()) { return nullptr; }
		return &it->second;
	}

  private:
	void loadChunks(glm::ivec2 centerChunk);
	void unloadChunks(glm::ivec2 centerChunk);

	std::unordered_map<glm::ivec2, Chunk, ChunkHash> m_chunks{};
	glm::ivec2 m_lastCenterChunk{std::numeric_limits<int>::max()};
};
} // namespace resurgo