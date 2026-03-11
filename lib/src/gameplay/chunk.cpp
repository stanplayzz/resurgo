#include "resurgo/gameplay/chunk.hpp"
#include "resurgo/gameplay/iso_transform.hpp"
#include <algorithm>

namespace resurgo {
namespace {
constexpr auto maxHeight_v = 20;
constexpr auto sideFaceHeight_v = tileSize_v * 0.25f;
constexpr auto rightShadow_v = sf::Color{180, 180, 180};
constexpr auto leftShadow_v = sf::Color{100, 100, 100};
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
	auto pos = IsoTransform::tileToScreen({tile.position.x, tile.position.y});
	auto x = pos.x;
	auto y = pos.y;
	auto z = static_cast<float>(tile.position.z) * sideFaceHeight_v;

	if (tile.rightFaceHeight > 0) {
		auto sideH = static_cast<float>(tile.rightFaceHeight) * sideFaceHeight_v;

		auto st1 = sf::Vector2f{x, y + (tileSize_v * 0.25f) - z};
		auto st2 = sf::Vector2f{x + (tileSize_v * 0.5f), y + (tileSize_v * 0.5f) - z};
		auto sb1 = sf::Vector2f{st1.x, st1.y + sideH};
		auto sb2 = sf::Vector2f{st2.x, st2.y + sideH};

		auto rect = getTileRectFromId(0);
		rect.size.y = rect.size.y * 0.5f * static_cast<float>(tile.rightFaceHeight);

		createQuad(st1, st2, sb1, sb2, rect, rightShadow_v);
	}

	if (tile.leftFaceHeight > 0) {
		auto sideH = static_cast<float>(tile.leftFaceHeight) * sideFaceHeight_v;

		auto st1 = sf::Vector2f{x + (tileSize_v * 0.5f), y + (tileSize_v * 0.5f) - z};
		auto st2 = sf::Vector2f{x + tileSize_v, y + (tileSize_v * 0.25f) - z};
		auto sb1 = sf::Vector2f{st1.x, st1.y + sideH};
		auto sb2 = sf::Vector2f{st2.x, st2.y + sideH};

		auto rect = getTileRectFromId(0);
		rect.size.y = rect.size.y * 0.5f * static_cast<float>(tile.leftFaceHeight);

		createQuad(st1, st2, sb1, sb2, rect, leftShadow_v);
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

		auto pos = IsoTransform::tileToScreen({tile.position.x, tile.position.y});
		auto x = pos.x;
		auto y = pos.y;
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

void Chunk::drawChunkBorders(sf::RenderTarget& target) const {
	auto line = [&](sf::Vector2f a, sf::Vector2f b, sf::Color color = sf::Color::Red) {
		auto verts = std::array{sf::Vertex{.position = a, .color = color}, sf::Vertex{.position = b, .color = color}};
		target.draw(verts.data(), 2, sf::PrimitiveType::Lines);
	};

	auto tileCorners = [](sf::Vector2f pos, float z) {
		struct {
			sf::Vector2f top, right, bottom, left;
		} c;
		c.top = {pos.x + (tileSize_v * 0.5f), pos.y - z};
		c.right = {pos.x + tileSize_v, pos.y + (tileSize_v * 0.25f) - z};
		c.bottom = {pos.x + (tileSize_v * 0.5f), pos.y + (tileSize_v * 0.5f) - z};
		c.left = {pos.x, pos.y + (tileSize_v * 0.25f) - z};
		return c;
	};

	auto corners = [&](std::size_t index) {
		auto const& tile = m_tiles.at(index);
		auto pos = IsoTransform::tileToScreen({tile.position.x, tile.position.y});
		auto z = static_cast<float>(tile.position.z) * (tileSize_v * 0.25f);
		return tileCorners(pos, z);
	};

	for (std::size_t i = 0; i < chunkSize_v; i++) {
		auto tr = corners(i);
		auto tl = corners(i * chunkSize_v);
		auto br = corners((i * chunkSize_v) + (chunkSize_v - 1));
		auto bl = corners(((chunkSize_v - 1) * chunkSize_v) + i);

		line(tr.top, tr.right);
		line(tl.top, tl.left);
		line(br.right, br.bottom);
		line(bl.left, bl.bottom);
	}
}

} // namespace resurgo