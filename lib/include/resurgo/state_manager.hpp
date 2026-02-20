#pragma once
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/System/Time.hpp>
#include <clib/base_types.hpp>
#include <memory>
#include <utility>

namespace resurgo {
class State : public clib::Polymorphic {
  public:
	virtual void update(sf::Time time) = 0;
	virtual void draw(sf::RenderTarget& target) const = 0;

	[[nodiscard]] virtual auto clearColor() const -> sf::Color {
		return sf::Color::Black;
	}
};

class StateManager {
  public:
	void update(sf::Time time) {
		if (m_currentState) { m_currentState->update(time); }
	}

	void draw(sf::RenderTarget& target) const {
		if (m_currentState) { m_currentState->draw(target); }
	}

	void setState(std::unique_ptr<State> state) {
		m_currentState = std::move(state);
	}

	[[nodiscard]] auto clearColor() const -> sf::Color {
		return m_currentState ? m_currentState->clearColor() : sf::Color::Black;
	}

  private:
	std::unique_ptr<State> m_currentState{};
};

} // namespace resurgo