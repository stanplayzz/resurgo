#pragma once
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Window/Event.hpp>
#include <clib/base_types.hpp>
#include <clib/not_null.hpp>
#include <memory>
#include <utility>

namespace resurgo {
class App;
class State : public clib::Polymorphic {
  public:
	virtual auto update(sf::Time deltaTime) -> std::unique_ptr<State> = 0;
	virtual void draw(sf::RenderTarget& target) const = 0;
	virtual void handleInput(sf::Event const& event) = 0;

	[[nodiscard]] virtual auto clearColor() const -> sf::Color {
		return sf::Color::Black;
	}
};

class StateManager {
  public:
	void update(sf::Time deltaTime) {
		if (!m_currentState) { return; }
		if (auto next = m_currentState->update(deltaTime)) { m_currentState = std::move(next); }
	}

	void draw(sf::RenderTarget& target) const {
		if (m_currentState) { m_currentState->draw(target); }
	}

	void setState(std::unique_ptr<State> state) {
		m_currentState = std::move(state);
	}

	void handleInput(sf::Event const& event) {
		if (m_currentState) { m_currentState->handleInput(event); }
	}

	[[nodiscard]] auto clearColor() const -> sf::Color {
		return m_currentState ? m_currentState->clearColor() : sf::Color::Black;
	}

  private:
	std::unique_ptr<State> m_currentState{};
};

} // namespace resurgo