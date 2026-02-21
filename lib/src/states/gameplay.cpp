#include "resurgo/states/gameplay.hpp"
#include "resurgo/state_manager.hpp"

namespace resurgo::state {
Gameplay::Gameplay(clib::not_null<App const*> app) : m_app(app) {}

auto Gameplay::update(sf::Time /*deltaTime*/) -> std::unique_ptr<State> {
	return nullptr;
}

void Gameplay::draw(sf::RenderTarget& /*target*/) const {}

void Gameplay::handleInput(sf::Event const& /*event*/) {}

} // namespace resurgo::state