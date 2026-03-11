#include "resurgo/gameplay/world.hpp"

namespace resurgo {

World::World() = default;

void World::update(sf::Time deltaTime, sf::View& view) {
	m_chunkManager.update(view);
	m_player.update(deltaTime, m_chunkManager);
}

void World::draw(sf::RenderTarget& target, Settings const& settings) const {
	m_chunkManager.draw(target, settings);
	m_player.draw(target, settings);
}
void World::handleInput(sf::Event const& event) { m_player.handleInput(event); }
} // namespace resurgo