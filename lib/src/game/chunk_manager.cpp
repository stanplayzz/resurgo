#include "resurgo/game/chunk_manager.hpp"

namespace resurgo {
namespace {
constexpr auto renderDistance_v = 3;
}

void ChunkManager::update(glm::vec2 playerPos) {
	auto centerChunk = glm::ivec2{std::floor(playerPos.x / chunkSize_v / tileSize_v),
								  std::floor(playerPos.y / chunkSize_v / tileSize_v)};

	if (centerChunk != m_lastCenterChunk) {
		m_lastCenterChunk = centerChunk;
		loadChunks(centerChunk);
		unloadChunks(centerChunk);
	}
}

void ChunkManager::loadChunks(glm::ivec2 centerChunk) {
	for (auto y = -renderDistance_v; y <= renderDistance_v; y++) {
		for (auto x = -renderDistance_v; x <= renderDistance_v; x++) {
			auto coord = glm::ivec2{centerChunk.x + x, centerChunk.y + y};

			if (!m_chunks.contains(coord)) {
				auto chunk = Chunk{};

				chunk.setPosition(coord);
				chunk.init();

				m_chunks.emplace(coord, std::move(chunk));
			}
		}
	}
	for (auto y = -renderDistance_v; y <= renderDistance_v; y++) {
		for (auto x = -renderDistance_v; x <= renderDistance_v; x++) {
			auto coord = glm::ivec2{centerChunk.x + x, centerChunk.y + y};
			auto* right = m_chunks.contains({coord.x + 1, coord.y}) ? &m_chunks.at({coord.x + 1, coord.y}) : nullptr;
			auto* front = m_chunks.contains({coord.x, coord.y + 1}) ? &m_chunks.at({coord.x, coord.y + 1}) : nullptr;
			m_chunks.at(coord).generate(right, front);
		}
	}
}

void ChunkManager::unloadChunks(glm::ivec2 centerChunk) {
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

void ChunkManager::draw(engine::Renderer& renderer) const {
	for (auto const& [coord, chunk] : m_chunks) { chunk.draw(renderer); }
}

} // namespace resurgo