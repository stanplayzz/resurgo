#pragma once
#include "resurgo/state_manager.hpp"
#include <memory>

namespace resurgo::state {
class Gameplay : public State {
  public:
	explicit Gameplay(clib::not_null<App const*> app);

	auto update(sf::Time deltaTime) -> std::unique_ptr<State> override;
	void draw(sf::RenderTarget& target) const override;
	void handleInput(sf::Event const& event) override;

  private:
	clib::not_null<App const*> m_app;
};
} // namespace resurgo::state