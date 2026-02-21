#include "resurgo/gameplay/world.hpp"

namespace resurgo {

World::World() = default;

void World::update(sf::View& view) { m_chunkLoader.update(view); }

void World::draw(sf::RenderTarget& target) const { m_chunkLoader.draw(target); }
} // namespace resurgo