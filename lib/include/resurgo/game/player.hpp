#pragma once
#include "resurgo/engine/renderer.hpp"
#include "resurgo/engine/shapes/quad.hpp"
#include "resurgo/game/chunk_manager.hpp"

namespace resurgo {
class Player {
  public:
	Player();
	void update(float deltaTime, ChunkManager const& chunks);
	void draw(engine::Renderer& renderer) const;

	[[nodiscard]] auto getPosition() const -> glm::vec3 { return m_player.transform.position; }

  private:
	void handleInput();
	void animate(float deltaTime);
	void setSprite(int id, bool flipped);

	shape::Quad m_player{};
	bool m_flipped{};
	float m_animTime{};
	int m_currentFrame{};
	glm::vec2 m_moveDirection{};
};
} // namespace resurgo