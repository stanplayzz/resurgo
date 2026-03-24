#include "resurgo/game/chunk.hpp"
#include <PerlinNoise.hpp>

namespace resurgo {
namespace {
constexpr auto noiseScale_v = 0.07f;
constexpr auto terrainHeight_v = 5;
constexpr auto heightColorFactor_v = 0.1f;

constexpr auto getColorFromHeight(float z) -> Color {
	auto c = 1.f - ((terrainHeight_v - z) * heightColorFactor_v);
	return {.r = c, .g = c, .b = c};
}
} // namespace

void Chunk::init(std::uint32_t seed) {
	siv::PerlinNoise noise{seed};
	m_tiles.resize(chunkSize_v * chunkSize_v);

	for (std::size_t y = 0; y < chunkSize_v; y++) {
		for (std::size_t x = 0; x < chunkSize_v; x++) {
			auto& tile = m_tiles[(y * chunkSize_v) + x];

			auto worldX = (static_cast<float>(x) + (m_position.x * chunkSize_v)) * tileSize_v;
			auto worldY = (static_cast<float>(y) + (m_position.y * chunkSize_v)) * tileSize_v;

			tile.transform.position.x = worldX;
			tile.transform.position.y = worldY;

			auto n = noise.noise2D_01((worldX * noiseScale_v) / tileSize_v, (worldY * noiseScale_v) / tileSize_v);
			auto height = (n * (terrainHeight_v - 1)) + 1; // used to get more variation
			tile.transform.position.z = static_cast<float>(std::floor(height)) * tileSize_v;
		}
	}

	m_material = engine::Material{engine::Resources::instance().getShader("TerrainShader")};
	m_material.setTexture(engine::Resources::instance().loadTexture(ASSETS_DIR "/images/tileset.png"));
}

void Chunk::generate(Chunk const* rightChunk, Chunk const* frontChunk) {
	m_geometry.clear();

	for (auto const& tile : m_tiles) {
		auto tx = tile.transform.position.x;
		auto ty = tile.transform.position.y;
		auto tz = tile.transform.position.z;

		auto p1 = glm::vec3{tx, ty, tz};
		auto p2 = glm::vec3{tx + tileSize_v, ty, tz};
		auto p3 = glm::vec3{tx + tileSize_v, ty + tileSize_v, tz};
		auto p4 = glm::vec3{tx, ty + tileSize_v, tz};

		createQuad(p1, p2, p3, p4, getColorFromHeight(tz / tileSize_v));
		createSides(tile, rightChunk, frontChunk);
	}

	m_geometry.upload();
}

void Chunk::createSides(Tile const& tile, Chunk const* rightChunk, Chunk const* frontChunk) {
	auto i = &tile - m_tiles.data();
	auto x = i % chunkSize_v;
	auto y = i / chunkSize_v;

	auto tz = tile.transform.position.z;
	auto tx = tile.transform.position.x;
	auto ty = tile.transform.position.y;

	auto const* right = [&]() -> Tile const* {
		if (x + 1 < chunkSize_v) { return &m_tiles[static_cast<std::size_t>((y * chunkSize_v) + (x + 1))]; }
		if (rightChunk) { return &rightChunk->tileAt(static_cast<std::size_t>(y * chunkSize_v)); }
		return nullptr;
	}();
	if (right && tz != right->transform.position.z) {
		auto lowerZ = std::min(tz, right->transform.position.z);
		auto higherZ = std::max(tz, right->transform.position.z);

		glm::vec3 p1{tx + tileSize_v, ty + tileSize_v, lowerZ};
		glm::vec3 p2{tx + tileSize_v, ty + tileSize_v, higherZ};
		glm::vec3 p3{tx + tileSize_v, ty, higherZ};
		glm::vec3 p4{tx + tileSize_v, ty, lowerZ};

		if (tz > right->transform.position.z) {
			createQuad(p1, p2, p3, p4, getColorFromHeight(higherZ / tileSize_v));
		} else {
			createQuad(p4, p3, p2, p1, getColorFromHeight(higherZ / tileSize_v));
		}
	}

	auto const* front = [&]() -> Tile const* {
		if (y + 1 < chunkSize_v) { return &m_tiles[static_cast<std::size_t>(((y + 1) * chunkSize_v) + x)]; }
		if (frontChunk) { return &frontChunk->tileAt(static_cast<std::size_t>(x)); }
		return nullptr;
	}();
	if (front && tz != front->transform.position.z) {
		auto lowerZ = std::min(tz, front->transform.position.z);
		auto higherZ = std::max(tz, front->transform.position.z);

		glm::vec3 p1{tx, ty + tileSize_v, lowerZ};
		glm::vec3 p2{tx, ty + tileSize_v, higherZ};
		glm::vec3 p3{tx + tileSize_v, ty + tileSize_v, higherZ};
		glm::vec3 p4{tx + tileSize_v, ty + tileSize_v, lowerZ};

		if (tz > front->transform.position.z) {
			createQuad(p1, p2, p3, p4, getColorFromHeight(higherZ / tileSize_v));
		} else {
			createQuad(p4, p3, p2, p1, getColorFromHeight(higherZ / tileSize_v));
		}
	}
}

void Chunk::createQuad(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec3 p4, Color color) {
	auto normal = glm::normalize(glm::cross(p2 - p1, p3 - p1));

	auto width = glm::length(p4 - p1) / tileSize_v;
	auto height = glm::length(p2 - p1) / tileSize_v;

	auto v1 = engine::Vertex{.position = p1, .normal = normal, .texCoords = {0, 0}, .color = color.toVec3()};
	auto v2 = engine::Vertex{.position = p2, .normal = normal, .texCoords = {0, height}, .color = color.toVec3()};
	auto v3 = engine::Vertex{.position = p3, .normal = normal, .texCoords = {width, height}, .color = color.toVec3()};
	auto v4 = engine::Vertex{.position = p4, .normal = normal, .texCoords = {width, 0}, .color = color.toVec3()};

	m_geometry.addQuad(v1, v2, v3, v4);
}

void Chunk::draw(engine::Renderer& renderer) const { renderer.draw(&m_geometry, &m_material, m_transform); }
} // namespace resurgo