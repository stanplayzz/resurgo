#include "resurgo/gameplay/scene.hpp"
#include "resurgo/app.hpp"
#include "resurgo/resources.hpp"

namespace resurgo {
Scene::Scene(clib::not_null<App const*> app) : m_app(app) {
	// pre load assets
	Resources::instance().load<sf::Texture>("images/tileset.png");

	m_view = sf::View(sf::FloatRect{{}, sf::Vector2f{app->window().getSize()}});
}

auto Scene::update(sf::Time /*deltaTime*/) -> std::unique_ptr<State> {
	auto const& window = m_app->window();
	if (m_panning) {
		auto mouse = sf::Mouse::getPosition(window);
		m_view.move(window.mapPixelToCoords(m_panningPosition) - window.mapPixelToCoords(mouse));
		m_panningPosition = mouse;
	}

	m_world.update(m_view);

	return nullptr;
}

void Scene::draw(sf::RenderTarget& target) const {
	target.setView(m_view);
	m_world.draw(target);
}

void Scene::handleInput(sf::Event const& event) {
	if (auto const* resized = event.getIf<sf::Event::Resized>()) { m_view.setSize({resized->size}); }

	if (auto const* mouse = event.getIf<sf::Event::MouseButtonPressed>()) {
		if (mouse->button == sf::Mouse::Button::Right) {
			m_panningPosition = mouse->position;
			m_panning = true;
		}
	}

	if (event.is<sf::Event::MouseButtonReleased>()) { m_panning = false; }
}
} // namespace resurgo