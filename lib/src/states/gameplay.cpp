#include "resurgo/states/gameplay.hpp"
#include "resurgo/state_manager.hpp"

namespace resurgo::state {
Gameplay::Gameplay(clib::not_null<App const*> app) : m_app(app), m_scene(app) {}

auto Gameplay::update(sf::Time deltaTime) -> std::unique_ptr<State> {
	m_scene.update(deltaTime);
	return nullptr;
}

void Gameplay::draw(sf::RenderTarget& target) const { m_scene.draw(target); }

void Gameplay::handleInput(sf::Event const& event) { m_scene.handleInput(event); }

} // namespace resurgo::state