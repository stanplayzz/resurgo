#pragma once
#include "resurgo/gameplay/chunk_manager.hpp"
#include "resurgo/gameplay/settings.hpp"
#include <SFML/Graphics.hpp>

namespace resurgo {
class Player {
  public:
	explicit Player();

	void update(sf::Time deltaTime, ChunkManager const& chunks);
	void draw(sf::RenderTarget& target, Settings const& settings) const;
	void handleInput(sf::Event const& event);

  private:
	sf::Sprite m_player;

	sf::FloatRect m_hitbox{};

	sf::Vector2f m_moveDirection{};
	int m_elevation{};
};
} // namespace resurgo