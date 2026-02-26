#pragma once
#include "resurgo/gameplay/entity.hpp"
#include "resurgo/gameplay/tile.hpp"
#include <SFML/Graphics.hpp>

namespace resurgo {
constexpr auto chunkSize_v = 8;

struct Layer {
	std::array<Tile, chunkSize_v * chunkSize_v> tiles{};
	sf::VertexArray vertices{};
};

struct ChunkData {
	std::vector<Layer> layers{};
	sf::Vector2i position{};
};

class Chunk : public sf::Drawable {
  public:
	void generate(ChunkData const& data);

	template <typename T>
	void addEntityAt(sf::Vector2i chunkPosition = {}) {
		m_entities.push_back(std::make_unique<T>());
		m_entities.back()->setPosition(chunkPosition);
	}

	void removeEntityAt(sf::Vector2i chunkPosition) {
		std::erase_if(m_entities, [&](auto const& e) {
			return e->getPosition() == chunkPosition;
		});
	}

	[[nodiscard]] auto position() const -> sf::Vector2i { return m_position; }
	void setPosition(sf::Vector2i position) { m_position = position; }

	void draw(sf::RenderTarget& target, sf::RenderStates states = {}) const override;

  private:
	sf::Vector2i m_position{};
	std::vector<Layer> m_layers{};
	std::vector<std::unique_ptr<Entity>> m_entities{};
};
} // namespace resurgo