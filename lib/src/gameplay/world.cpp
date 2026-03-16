#include "resurgo/gameplay/world.hpp"

namespace resurgo {

World::World() = default;

void World::update(sf::Time deltaTime, sf::RenderWindow const& window) {
	m_chunkManager.update(window.getView());
	m_player.update(deltaTime, m_chunkManager);
	m_tileCursor.update(deltaTime, window.mapPixelToCoords(sf::Mouse::getPosition(window)));
}

void World::draw(sf::RenderTarget& target, Settings const& settings) const {
	m_chunkManager.draw(target, settings);
	m_tileCursor.draw(target);
	m_player.draw(target, settings);
}
void World::handleInput(sf::Event const& event) { m_player.handleInput(event); }
} // namespace resurgo