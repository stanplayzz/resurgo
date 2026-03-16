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

	[[nodiscard]] auto getHitbox() const -> sf::FloatRect { return m_hitbox; }

  private:
	void setSprite(int id, bool flipped = false);
	void animate(sf::Time deltaTime);

	sf::Sprite m_player;

	int m_currentFrame{};
	float m_animTime{};
	bool m_flipped{};

	sf::FloatRect m_hitbox{};

	sf::Vector2f m_moveDirection{};
	int m_elevation{};
};
} // namespace resurgo