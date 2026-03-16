#pragma once
#include "resurgo/gameplay/UI/materials.hpp"
#include "resurgo/gameplay/player.hpp"
#include "resurgo/gameplay/settings.hpp"
#include "resurgo/gameplay/storage.hpp"
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

	Settings m_settings{};

	float zoomLevel{1.f};

	World m_world{};
	Storage m_storage{};

	Player m_player{};

	// UI Components
	std::optional<ui::Materials> m_materialsUI{};
};
} // namespace resurgo