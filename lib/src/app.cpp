#include "resurgo/app.hpp"
#include "resurgo/build_version.hpp"
#include "resurgo/states/entrypoint.hpp"
#include <SFML/Graphics.hpp>
#include <memory>

namespace resurgo {
App::App() {
	m_window.create(sf::VideoMode{{1280, 720}}, std::format("Resurgo {}", buildVersionStr_v));

	m_stateManager.setState(std::make_unique<state::Entrypoint>());
}

void App::run() {
	sf::Clock m_clock{};

	while (m_window.isOpen()) {
		while (auto const event = m_window.pollEvent()) {
			if (event->is<sf::Event::Closed>()) { m_window.close(); }
		}

		auto deltaTime = m_clock.getElapsedTime();

		m_stateManager.update(deltaTime);

		m_window.clear(m_stateManager.clearColor());

		m_stateManager.draw(m_window);

		m_window.display();
	}
}
} // namespace resurgo