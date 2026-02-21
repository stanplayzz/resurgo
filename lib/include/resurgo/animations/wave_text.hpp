#pragma once
#include "resurgo/animation.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>

namespace resurgo::animation {
class WaveText : public Animation {
  public:
	explicit WaveText(sf::Font const& font, std::string const& str, unsigned charSize);

	void update(sf::Time deltaTime) override;
	void draw(sf::RenderTarget& target) const override;

	void setPosition(sf::Vector2f position) override {
		auto delta = position - m_position;
		for (auto& letter : m_letters) { letter.setPosition(letter.getPosition() + delta); }
		m_position = position;
	}
	[[nodiscard]] auto getPosition() const -> sf::Vector2f override {
		return m_position;
	}

  private:
	std::vector<sf::Text> m_letters{};
	sf::Vector2f m_position{};
	sf::Time m_elapsed{};
};
} // namespace resurgo::animation