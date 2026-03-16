#include "resurgo/gameplay/tile_cursor.hpp"
#include "resurgo/gameplay/iso_transform.hpp"
#include <numbers>

namespace resurgo {
void TileCursor::createCursor() {
	m_cursor.setPointCount(4);
	m_cursor.setPoint(0, {tileSize_v * 0.5f, 0});
	m_cursor.setPoint(1, {tileSize_v, tileSize_v * 0.25f});
	m_cursor.setPoint(2, {tileSize_v * 0.5f, tileSize_v * 0.5f});
	m_cursor.setPoint(3, {0, tileSize_v * 0.25f});

	m_cursor.setFillColor(sf::Color::Transparent);
	m_cursor.setOutlineThickness(3);
}

void TileCursor::update(sf::Time deltaTime, sf::Vector2f mousePos) {
	m_hoveredTile = IsoTransform::getTileFromPoint(mousePos, *m_chunks);

	m_pulse += deltaTime.asSeconds() * 3;
	if (m_pulse > 2 * std::numbers::pi_v<float>) { m_pulse -= 2 * std::numbers::pi_v<float>; }
}

void TileCursor::draw(sf::RenderTarget& target) const {
	if (!m_hoveredTile) { return; }

	auto screenPos = IsoTransform::tileToScreen(m_hoveredTile->position);
	auto pulse = (std::sin(m_pulse) * 0.5f) + 0.5f;

	auto cursor = sf::ConvexShape{m_cursor};
	cursor.setPosition(screenPos);

	cursor.setOutlineColor(sf::Color{255, 255, 255, static_cast<std::uint8_t>(100 + (pulse * 155))});

	target.draw(cursor);
}

} // namespace resurgo