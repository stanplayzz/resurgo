#pragma once
#include "resurgo/engine/shapes/quad.hpp"
#include "resurgo/game/chunk_manager.hpp"
#include <clib/not_null.hpp>

namespace resurgo {
class Cursor {
  public:
	Cursor(clib::not_null<ChunkManager const*> chunkManager);
	void update(float deltaTime, engine::Camera const& camera);
	void draw(engine::Renderer& renderer) const;

  private:
	clib::not_null<ChunkManager const*> m_chunkManager;
	shape::Quad m_cursor{};
	float m_elapsedTime{};
	glm::vec3 m_target{};
};
} // namespace resurgo