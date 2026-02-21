#pragma once
#include "resurgo/animations/wave_text.hpp"
#include "resurgo/resources.hpp"
#include "resurgo/state_manager.hpp"
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <memory>
#include <optional>

namespace resurgo::state {
class Entrypoint : public State {
  public:
	explicit Entrypoint(clib::not_null<App const*> app);

	// functions inherited from State
	auto update(sf::Time deltaTime) -> std::unique_ptr<State> override;
	void draw(sf::RenderTarget& target) const override;
	void handleInput(sf::Event const& event) override;

  private:
	clib::not_null<App const*> m_app;

	sf::Sprite m_background{*Resources::instance().load<sf::Texture>("images/entrypoint_background.png")};
	sf::Sprite m_planet{*Resources::instance().load<sf::Texture>("images/entrypoint_planet.png")};

	sf::Text m_playText{*Resources::instance().load<sf::Font>("fonts/exo2.ttf")};
	sf::Vector2f m_playTextPosition{50.f, 200.f};

	std::optional<animation::WaveText> m_titleText{};

	std::unique_ptr<State> targetState{};
};
} // namespace resurgo::state