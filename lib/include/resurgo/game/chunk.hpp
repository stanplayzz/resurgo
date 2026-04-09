#pragma once
#include "resurgo/engine/dynamic_geometry.hpp"
#include "resurgo/engine/renderer.hpp"
#include "resurgo/game/tile.hpp"

namespace resurgo {
constexpr auto chunkSize_v = 16;
constexpr auto terrainHeight_v = 5;

class Chunk {
  public:
	void init(std::uint32_t seed = 12345);
	void generate(Chunk const* rightChunk, Chunk const* frontChunk);

	void draw(engine::Renderer& renderer) const;

	void setPosition(glm::ivec2 position) { m_position = position; }

	[[nodiscard]] auto tileAt(std::size_t index) const -> Tile const& { return m_tiles.at(index); }

  private:
	void createSides(Tile const& tile, Chunk const* rightChunk, Chunk const* frontChunk);

	void createQuad(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec3 p4, Color color = Color::White);

	std::vector<Tile> m_tiles{};
	glm::vec2 m_position{};

	engine::DynamicGeometry m_geometry{};
	engine::Material m_material{};
	engine::Transform m_transform{};
};
} // namespace resurgo