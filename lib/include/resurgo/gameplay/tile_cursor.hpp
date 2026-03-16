#pragma once
#include "resurgo/gameplay/chunk_manager.hpp"
#include <SFML/Graphics.hpp>
#include <clib/not_null.hpp>

namespace resurgo {
class TileCursor {
  public:
	explicit TileCursor(clib::not_null<ChunkManager const*> chunks) : m_chunks(chunks) { createCursor(); }
	void update(sf::Time deltaTime, sf::Vector2f mousePos);
	void draw(sf::RenderTarget& target) const;

	[[nodiscard]] auto getHoveredTile() const -> Tile const*;

  private:
	void createCursor();

	clib::not_null<ChunkManager const*> m_chunks;
	Tile const* m_hoveredTile{};
	float m_pulse{};
	sf::ConvexShape m_cursor{};
};
} // namespace resurgo