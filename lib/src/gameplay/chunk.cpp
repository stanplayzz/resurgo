#include "resurgo/gameplay/chunk.hpp"
#include <algorithm>

namespace resurgo {
namespace {
constexpr auto maxHeight_v = 20;
constexpr auto sideFaceHeight_v = tileSize_v * 0.25f;
} // namespace

void Chunk::generate(siv::PerlinNoise noise) {
	m_tiles.resize(chunkSize_v * chunkSize_v);
	for (std::size_t y = 0; y < chunkSize_v; y++) {
		for (std::size_t x = 0; x < chunkSize_v; x++) {
			auto& tile = m_tiles.at((y * chunkSize_v) + x);

			auto worldX = static_cast<int>(x) + (m_position.x * chunkSize_v);
			auto worldY = static_cast<int>(y) + (m_position.y * chunkSize_v);

			tile.position.x = worldX;
			tile.position.y = worldY;
			auto n = noise.noise2D_01(worldX * 0.02, worldY * 0.02);
			tile.position.z = static_cast<int>(n * maxHeight_v);
		}
	}
}

void Chunk::computeFaces(std::function<Chunk*(sf::Vector2i)> const& getNeighbour) {
	for (std::size_t y = 0; y < chunkSize_v; y++) {
		for (std::size_t x = 0; x < chunkSize_v; x++) {
			auto& tile = m_tiles.at((y * chunkSize_v) + x);

			auto leftZ = 0;
			auto rightZ = 0;

			// x+1 neighbour
			if (x < chunkSize_v - 1) {
				leftZ = m_tiles.at((y * chunkSize_v) + (x + 1)).position.z;
			} else {
				auto* n = getNeighbour({m_position.x + 1, m_position.y});
				if (n) { leftZ = n->tileAt({0, static_cast<int>(y)}).position.z; }
			}

			// y+1 neighbour
			if (y < chunkSize_v - 1) {
				rightZ = m_tiles.at(((y + 1) * chunkSize_v) + x).position.z;
			} else {
				auto* n = getNeighbour({m_position.x, m_position.y + 1});
				if (n) { rightZ = n->tileAt({static_cast<int>(x), 0}).position.z; }
			}

			tile.leftFaceHeight = std::max(0, tile.position.z - leftZ);
			tile.rightFaceHeight = std::max(0, tile.position.z - rightZ);
		}
	}

	generateVertexArray();
}

void Chunk::createQuad(sf::Vector2f p1, sf::Vector2f p2, sf::Vector2f p3, sf::Vector2f p4, sf::FloatRect rect,
					   sf::Color color) {

	auto texTop = sf::Vector2f{rect.position.x, rect.position.y};
	auto texRight = sf::Vector2f{rect.position.x + rect.size.x, rect.position.y};
	auto texBottom = sf::Vector2f{rect.position.x + rect.size.x, rect.position.y + rect.size.y};
	auto texLeft = sf::Vector2f{rect.position.x, rect.position.y + rect.size.y};

	m_vertexArray.append({.position = p1, .color = color, .texCoords = texTop});
	m_vertexArray.append({.position = p2, .color = color, .texCoords = texRight});
	m_vertexArray.append({.position = p3, .color = color, .texCoords = texLeft});
	m_vertexArray.append({.position = p2, .color = color, .texCoords = texRight});
	m_vertexArray.append({.position = p4, .color = color, .texCoords = texBottom});
	m_vertexArray.append({.position = p3, .color = color, .texCoords = texLeft});
}

void Chunk::generateSideFaces(Tile const& tile) {
	auto x = static_cast<float>(tile.position.x - tile.position.y) * (tileSize_v * 0.5f);
	auto y = static_cast<float>(tile.position.x + tile.position.y) * (tileSize_v * 0.25f);
	auto z = static_cast<float>(tile.position.z) * sideFaceHeight_v;

	if (tile.rightFaceHeight > 0) {
		auto sideH = static_cast<float>(tile.rightFaceHeight) * sideFaceHeight_v;

		auto st1 = sf::Vector2f{x, y + (tileSize_v * 0.25f) - z};
		auto st2 = sf::Vector2f{x + (tileSize_v * 0.5f), y + (tileSize_v * 0.5f) - z};
		auto sb1 = sf::Vector2f{st1.x, st1.y + sideH};
		auto sb2 = sf::Vector2f{st2.x, st2.y + sideH};

		createQuad(st1, st2, sb1, sb2, getTileRectFromId(1));
	}

	if (tile.leftFaceHeight > 0) {
		auto sideH = static_cast<float>(tile.leftFaceHeight) * sideFaceHeight_v;

		auto st1 = sf::Vector2f{x + (tileSize_v * 0.5f), y + (tileSize_v * 0.5f) - z};
		auto st2 = sf::Vector2f{x + tileSize_v, y + (tileSize_v * 0.25f) - z};
		auto sb1 = sf::Vector2f{st1.x, st1.y + sideH};
		auto sb2 = sf::Vector2f{st2.x, st2.y + sideH};

		auto shade = sf::Color{180, 180, 180};

		createQuad(st1, st2, sb1, sb2, getTileRectFromId(1), shade);
	}
}

void Chunk::generateVertexArray() {
	m_vertexArray.setPrimitiveType(sf::PrimitiveType::Triangles);
	m_vertexArray.clear();

	// sort tiles
	auto sorted = m_tiles;
	std::ranges::sort(sorted, [](Tile const& a, Tile const& b) {
		return (a.position.x + a.position.y) < (b.position.x + b.position.y);
	});

	for (auto const& tile : sorted) {
		generateSideFaces(tile);

		auto x = static_cast<float>(tile.position.x - tile.position.y) * (tileSize_v * 0.5f);
		auto y = static_cast<float>(tile.position.x + tile.position.y) * (tileSize_v * 0.25f);
		auto z = static_cast<float>(tile.position.z) * sideFaceHeight_v;

		// vertices
		auto top = sf::Vector2f{x + (tileSize_v * 0.5f), y - z};
		auto right = sf::Vector2f{x + tileSize_v, y + (tileSize_v * 0.25f - z)};
		auto bottom = sf::Vector2f{x + (tileSize_v * 0.5f), y + (tileSize_v * 0.5f - z)};
		auto left = sf::Vector2f{x, y + (tileSize_v * 0.25f - z)};

		createQuad(top, right, left, bottom, getTileRectFromId(0));
	}
}

void Chunk::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	states.texture = Resources::instance().get<sf::Texture>("images/tileset.png").get();
	target.draw(m_vertexArray, states);
}
} // namespace resurgo