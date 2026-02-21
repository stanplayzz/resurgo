#pragma once
#include "resurgo/state_manager.hpp"
#include <SFML/Graphics.hpp>

namespace resurgo {
class App {
  public:
	explicit App();
	void run();

	[[nodiscard]] auto window() const -> sf::RenderWindow const& {
		return m_window;
	}

  private:
	sf::RenderWindow m_window{};

	StateManager m_stateManager{};
};
} // namespace resurgo