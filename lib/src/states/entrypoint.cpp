#include "resurgo/states/entrypoint.hpp"
#include "resurgo/animations/wave_text.hpp"
#include "resurgo/app.hpp"
#include "resurgo/resources.hpp"
#include "resurgo/state_manager.hpp"
#include "resurgo/states/gameplay.hpp"
#include <memory>

namespace resurgo::state {
namespace {
constexpr auto planetRotateSpeed_v = 5.f;
}

Entrypoint::Entrypoint(clib::not_null<App const*> app) : m_app(app) {
	auto const& window = m_app->window();

	auto scale = sf::Vector2f{
		static_cast<float>(window.getSize().x) / static_cast<float>(m_background.getTexture().getSize().x),
		static_cast<float>(window.getSize().y) / static_cast<float>(m_background.getTexture().getSize().y)};
	m_background.setScale(scale);

	m_planet.setScale(scale);
	m_planet.setOrigin(m_planet.getLocalBounds().getCenter());
	m_planet.setPosition(window.getView().getCenter() + sf::Vector2f{0.f, window.getView().getSize().y * 0.5f});

	m_titleText.emplace(animation::WaveText(*Resources::instance().load<sf::Font>("fonts/exo2.ttf"), "Resurgo", 64));
	m_titleText->setPosition({50.f, 50.f});

	m_playText.setString("PLAY");
	m_playText.setCharacterSize(50);
	m_playText.setPosition(m_playTextPosition);
}

auto Entrypoint::update(sf::Time deltaTime) -> std::unique_ptr<State> {
	auto const& window = m_app->window();

	m_titleText->update(deltaTime);

	m_planet.rotate(sf::degrees(planetRotateSpeed_v * deltaTime.asSeconds()));

	if (m_playText.getGlobalBounds().contains(window.mapPixelToCoords(sf::Mouse::getPosition(window)))) {
		auto targetPos = m_playTextPosition + sf::Vector2f{10.f, 0.f};
		auto t = 10.f * deltaTime.asSeconds();
		m_playText.setPosition(m_playText.getPosition() + (targetPos - m_playText.getPosition()) * t);
	} else {
		auto t = 10.f * deltaTime.asSeconds();
		m_playText.setPosition(m_playText.getPosition() + (m_playTextPosition - m_playText.getPosition()) * t);
	}

	return std::move(targetState);
}

void Entrypoint::draw(sf::RenderTarget& target) const {
	target.draw(m_background);
	target.draw(m_planet);
	m_titleText->draw(target);
	target.draw(m_playText);
}

void Entrypoint::handleInput(sf::Event const& event) {
	auto const& window = m_app->window();

	if (auto const* mouse = event.getIf<sf::Event::MouseButtonReleased>()) {
		if (mouse->button == sf::Mouse::Button::Left) {
			if (m_playText.getGlobalBounds().contains(window.mapPixelToCoords(mouse->position))) {
				targetState = std::make_unique<Gameplay>(m_app);
			}
		}
	}
}
} // namespace resurgo::state