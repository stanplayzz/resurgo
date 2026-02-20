#pragma once
#include "resurgo/animations/wave_text.hpp"
#include "resurgo/state_manager.hpp"
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <optional>

namespace resurgo::state {
class Entrypoint : public State {
  public:
	explicit Entrypoint();

	// functions inherited from State
	void update(sf::Time time) override;
	void draw(sf::RenderTarget& target) const override;
	[[nodiscard]] auto clearColor() const -> sf::Color override {
		return sf::Color::Blue;
	}

  private:
	sf::Texture m_backgroundTexture{};
	sf::Sprite m_background{m_backgroundTexture};

	std::optional<animation::WaveText> m_titleText{};
	sf::Font m_titleFont{};
};
} // namespace resurgo::state