#pragma once
#include "resurgo/gameplay/chunk_loader.hpp"
#include <SFML/Graphics.hpp>

namespace resurgo {
class World {
  public:
	explicit World();

	void update(sf::View& view);
	void draw(sf::RenderTarget& target) const;

  private:
	ChunkLoader m_chunkLoader{};
};
} // namespace resurgo