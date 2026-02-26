#pragma once
#include "resurgo/gameplay/storage.hpp"
#include "resurgo/resources.hpp"
#include <SFML/Graphics.hpp>
#include <clib/not_null.hpp>

namespace resurgo::ui {

class Materials : public sf::Drawable {
  public:
	explicit Materials(clib::not_null<Storage const*> storage, sf::Vector2f windowSize);
	void update();
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

  private:
	std::vector<sf::Text> m_texts{};
	sf::Sprite m_body{*Resources::instance().load<sf::Texture>("images/material_bar.png")};

	clib::not_null<Storage const*> m_storage;
};
} // namespace resurgo::ui