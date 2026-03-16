#include "resurgo/gameplay/scene.hpp"
#include "resurgo/app.hpp"
#include "resurgo/resources.hpp"

namespace resurgo {
namespace {
constexpr auto viewMoveSpeed_v = 5;
}

Scene::Scene(clib::not_null<App const*> app) : m_app(app) {
	// pre load assets
	Resources::instance().load<sf::Texture>("images/tileset.png");

	m_view = sf::View(sf::FloatRect{{}, sf::Vector2f{app->window().getSize()}});
	m_storage.load();

	m_materialsUI.emplace(&m_storage, sf::Vector2f{m_app->window().getSize()});
}

auto Scene::update(sf::Time deltaTime) -> std::unique_ptr<State> {
	auto center = m_view.getCenter();
	auto target = m_world.getPlayerHitbox().getCenter() - sf::Vector2f{0, m_view.getSize().y * 0.2f};
	auto t = 1.f - std::exp(-viewMoveSpeed_v * deltaTime.asSeconds());
	m_view.setCenter(center + (target - center) * t);

	m_world.update(deltaTime, m_app->window());

	m_materialsUI->update();

	return nullptr;
}

void Scene::draw(sf::RenderTarget& target) const {
	target.setView(m_view);
	m_world.draw(target, m_settings);

	target.setView(target.getDefaultView());
	m_materialsUI->draw(target, {});
	target.setView(m_view);
}

void Scene::handleInput(sf::Event const& event) {
	auto const& window = m_app->window();
	if (auto const* resized = event.getIf<sf::Event::Resized>()) { m_view.setSize({resized->size}); }

	if (auto const* mouse = event.getIf<sf::Event::MouseWheelScrolled>()) {
		auto step = (mouse->delta > 0) ? 0.9f : 1.1f;
		auto newZoom = zoomLevel * step;

		newZoom = std::clamp(newZoom, 0.1f, 5.f);
		zoomLevel = newZoom;

		m_view.setSize(window.getDefaultView().getSize() * newZoom);
	}

	// SETTINGS
	if (auto const* key = event.getIf<sf::Event::KeyPressed>()) {
		if (key->scancode == sf::Keyboard::Scancode::Z && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LControl)) {
			m_settings.drawChunkBorders = !m_settings.drawChunkBorders;
		}
		if (key->scancode == sf::Keyboard::Scancode::X && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LControl)) {
			m_settings.drawHitboxes = !m_settings.drawHitboxes;
		}
	}

	m_world.handleInput(event);
}
} // namespace resurgo