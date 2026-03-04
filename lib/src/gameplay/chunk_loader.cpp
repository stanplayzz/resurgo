#include "resurgo/gameplay/chunk_loader.hpp"
#include <cmath>

namespace resurgo {
namespace {
constexpr auto renderDistance_v = 4;
} // namespace

void ChunkLoader::update(sf::View const& view) {
	auto center = view.getCenter();

	// make sure to match isometric chunk generation
	auto x = (center.x / (tileSize_v * 0.5f) + center.y / (tileSize_v * 0.25f)) * 0.5f;
	auto y = (center.y / (tileSize_v * 0.25f) - center.x / (tileSize_v * 0.5f)) * 0.5f;

	auto centerChunk = sf::Vector2i{
		static_cast<int>(std::floor(x / chunkSize_v)),
		static_cast<int>(std::floor(y / chunkSize_v)),
	};

	if (centerChunk != m_lastCenterChunk) {
		m_lastCenterChunk = centerChunk;
		loadChunks(centerChunk);
		unloadChunks(centerChunk);
		computeChunkFaces();
	}
}

void ChunkLoader::loadChunks(sf::Vector2i centerChunk) {
	for (auto y = -renderDistance_v; y <= renderDistance_v; y++) {
		for (auto x = -renderDistance_v; x <= renderDistance_v; x++) {
			auto coord = sf::Vector2i{centerChunk.x + x, centerChunk.y + y};

			if (!m_chunks.contains(coord)) {
				auto chunk = Chunk{};

				chunk.setPosition(coord);
				chunk.generate(m_noise);

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

void ChunkLoader::computeChunkFaces() {
	for (auto& [coord, chunk] : m_chunks) {
		chunk.computeFaces([&](sf::Vector2i neighbour) -> Chunk* {
			auto it = m_chunks.find(neighbour);
			return it != m_chunks.end() ? &it->second : nullptr;
		});
	}
}

void ChunkLoader::draw(sf::RenderTarget& target) const {
	auto sorted = std::vector<Chunk const*>{};
	sorted.reserve(m_chunks.size());
	for (auto const& [coord, chunk] : m_chunks) { sorted.push_back(&chunk); }
	std::ranges::sort(sorted, [](Chunk const* a, Chunk const* b) {
		return (a->position().x + a->position().y) < (b->position().x + b->position().y);
	});
	for (auto const* chunk : sorted) { chunk->draw(target); }
}
} // namespace resurgo