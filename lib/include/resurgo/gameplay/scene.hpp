#pragma once
#include "resurgo/gameplay/world.hpp"
#include "resurgo/state_manager.hpp"
#include <SFML/Graphics.hpp>

namespace resurgo {
class Scene {
  public:
	explicit Scene(clib::not_null<App const*> app);

	auto update(sf::Time deltaTime) -> std::unique_ptr<State>;
	void draw(sf::RenderTarget& target) const;
	void handleInput(sf::Event const& event);

  private:
	clib::not_null<App const*> m_app;

	sf::View m_view{};

	// view transformations
	sf::Vector2i m_panningPosition{};
	bool m_panning{};
	float zoomLevel{1.f};

	World m_world{};
};
} // namespace resurgo