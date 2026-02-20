#pragma once
#include "resurgo/state_manager.hpp"
#include <SFML/Graphics.hpp>

namespace resurgo {
class App {
  public:
	explicit App();

	void run();

  private:
	sf::RenderWindow m_window{};

	StateManager m_stateManager{};
};
} // namespace resurgo