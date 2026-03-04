#pragma once
#include "resurgo/gameplay/tile.hpp"
#include "resurgo/utils/perlin_noise.hpp"
#include <SFML/Graphics.hpp>

namespace resurgo {
constexpr auto chunkSize_v = 8;

class Chunk : public sf::Drawable {
  public:
	void generate(siv::PerlinNoise noise);
	void computeFaces(std::function<Chunk*(sf::Vector2i)> const& getNeighbour);

	[[nodiscard]] auto position() const -> sf::Vector2i { return m_position; }
	void setPosition(sf::Vector2i position) { m_position = position; }

	[[nodiscard]] auto tileAt(sf::Vector2i coord) const -> Tile {
		for (auto const& tile : m_tiles) {
			if (tile.position.x == coord.x && tile.position.y == coord.y) { return tile; }
		}
		return {};
	}

	void draw(sf::RenderTarget& target, sf::RenderStates states = {}) const override;

  private:
	void createQuad(sf::Vector2f p1, sf::Vector2f p2, sf::Vector2f p3, sf::Vector2f p4, sf::FloatRect rect,
					sf::Color color = sf::Color::White);

	void generateVertexArray();
	void generateSideFaces(Tile const& tile);

	sf::Vector2i m_position{};
	std::vector<Tile> m_tiles{};
	sf::VertexArray m_topFaces{};
	sf::VertexArray m_sidesFaces{};
	sf::VertexArray m_vertexArray{};
};
} // namespace resurgo