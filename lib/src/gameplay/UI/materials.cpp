#include "resurgo/gameplay/UI/materials.hpp"

namespace resurgo::ui {
Materials::Materials(clib::not_null<Storage const*> storage, sf::Vector2f windowSize) : m_storage(storage) {
	auto factor = (windowSize.x * 0.3f) / m_body.getGlobalBounds().size.x; // 30% window width
	m_body.setScale({factor, factor});
	m_body.setPosition(windowSize - m_body.getGlobalBounds().size);

	for (std::size_t i = 0; i < storage->getContent().size(); i++) {
		m_texts.emplace_back(*Resources::instance().get<sf::Font>("fonts/exo2.ttf"));
		m_texts.back().setCharacterSize(32);
	}
}

void Materials::update() {
	auto const& content = m_storage->getContent();
	auto offset = 100.f;
	for (std::size_t i = 0; i < content.size(); i++) {
		auto& text = m_texts.at(i);
		text.setString(std::to_string(content.at(static_cast<Item>(i))));
		text.setOrigin(text.getLocalBounds().getCenter());
		text.setPosition(m_body.getGlobalBounds().getCenter() -
						 sf::Vector2f{(m_body.getGlobalBounds().size.x / 2.f) - offset, 0.f});
		offset += (m_body.getGlobalBounds().size.x - 100.f) / static_cast<float>(content.size());
	}
}

void Materials::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	target.draw(m_body, states);
	for (auto const& text : m_texts) { target.draw(text, states); }
}
} // namespace resurgo::ui