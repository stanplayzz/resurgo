#include "resurgo/states/entrypoint.hpp"
#include "resurgo/animations/wave_text.hpp"
#include "resurgo/resources.hpp"

namespace resurgo::state {
Entrypoint::Entrypoint() {

	m_titleText.emplace(animation::WaveText(*Resources::instance().load<sf::Font>("/fonts/exo2.ttf"), "Resurgo", 64));
	m_titleText->setPosition({50.f, 100.f});
}

void Entrypoint::update(sf::Time time) {
	m_titleText->update(time);
}

void Entrypoint::draw(sf::RenderTarget& target) const {
	m_titleText->draw(target);
}
} // namespace resurgo::state