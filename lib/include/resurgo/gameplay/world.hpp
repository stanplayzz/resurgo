#pragma once
#include "resurgo/gameplay/chunk_manager.hpp"
#include "resurgo/gameplay/player.hpp"
#include "resurgo/gameplay/settings.hpp"
#include "resurgo/gameplay/tile_cursor.hpp"
#include <SFML/Graphics.hpp>

namespace resurgo {
class World {
  public:
	explicit World();

	void update(sf::Time deltatime, sf::RenderWindow const& window);
	void draw(sf::RenderTarget& target, Settings const& settings) const;
	void handleInput(sf::Event const& event);

	[[nodiscard]] auto getPlayerHitbox() const -> sf::FloatRect { return m_player.getHitbox(); }

  private:
	ChunkManager m_chunkManager{};
	Player m_player{};
	TileCursor m_tileCursor{&m_chunkManager};
};
} // namespace resurgo