#include "resurgo/animations/wave_text.hpp"
#include <cmath>
#include <cstddef>

namespace resurgo::animation {
namespace {
constexpr auto speed_v = 3.f;
constexpr auto reach_v = 5.f;
constexpr auto letterSpacing_v = 1.1f;
} // namespace

WaveText::WaveText(sf::Font const& font, std::string const& str, unsigned const charSize) {
	auto x = 0.f;
	for (std::size_t i = 0; i < str.size(); i++) {
		auto letter = sf::Text{font};
		letter.setString(str.substr(i, 1));
		letter.setCharacterSize(charSize);

		letter.setPosition({x, 0.f});

		x += letter.getGlobalBounds().size.x * letterSpacing_v;
		m_letters.push_back(letter);
	}
}

void WaveText::update(sf::Time deltaTime) {
	m_elapsed += deltaTime;
	for (std::size_t i = 0; i < m_letters.size(); i++) {
		auto delta = std::sin((m_elapsed.asSeconds() * speed_v) + (static_cast<float>(i) * 0.4f)) * reach_v;

		auto pos = sf::Vector2f{m_letters[i].getPosition().x, m_position.y + delta};
		m_letters[i].setPosition(pos);
	}
}

void WaveText::draw(sf::RenderTarget& target) const {
	for (auto const& letter : m_letters) { target.draw(letter); }
}

} // namespace resurgo::animation