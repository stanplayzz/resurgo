#pragma once
#include <SFML/Graphics.hpp>
#include <clib/base_types.hpp>

namespace resurgo {
namespace {
constexpr auto entityTextureOffset_v = 32;

constexpr auto getEntityRect(std::size_t id, sf::Texture& tileset) {
	auto texuresPerRow = tileset.getSize().x / entityTextureOffset_v;
	auto row = id % texuresPerRow;
	auto col = id / texuresPerRow;

	return sf::IntRect{{static_cast<int>(row * entityTextureOffset_v), static_cast<int>(col * entityTextureOffset_v)},
					   {entityTextureOffset_v, entityTextureOffset_v}};
}
} // namespace

class Entity : public clib::Polymorphic, public sf::Drawable {
  public:
	explicit Entity(std::size_t id, sf::Texture& tileset) : m_sprite{tileset} {
		m_sprite.setTextureRect(getEntityRect(id, tileset));
	}

	virtual void update(sf::Time /*deltaTime*/) {}

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override { target.draw(m_sprite, states); }

	// sets position relative to chunk origin
	void setPosition(sf::Vector2i chunkPosition) { m_chunkPosition = chunkPosition; }
	// gets position relative to chunk origin
	[[nodiscard]] auto getPosition() const -> sf::Vector2i { return m_chunkPosition; }

  protected:
	sf::Vector2i m_chunkPosition{};
	sf::Sprite m_sprite;
};
} // namespace resurgo