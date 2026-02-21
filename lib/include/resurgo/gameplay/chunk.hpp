#pragma once
#include <SFML/Graphics.hpp>

namespace resurgo {
constexpr auto chunkSize_v = 8;

struct Layer {
	std::array<std::size_t, chunkSize_v * chunkSize_v> tiles{};
	sf::VertexArray vertices{};
};

struct ChunkData {
	std::vector<Layer> layers{};
	sf::Vector2i position{};
};

class Chunk : public sf::Drawable {
  public:
	void generate(ChunkData const& data);

	[[nodiscard]] auto position() const -> sf::Vector2i { return m_position; }
	void setPosition(sf::Vector2i position) { m_position = position; }

	void draw(sf::RenderTarget& target, sf::RenderStates states = {}) const override;

  private:
	sf::Vector2i m_position{};
	std::vector<Layer> m_layers{};
};
} // namespace resurgo