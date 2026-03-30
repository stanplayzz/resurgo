#pragma once
#include "resurgo/game/chunk_manager.hpp"
#include "resurgo/game/state_manager.hpp"

namespace resurgo {
class Gameplay : public State {
  public:
	Gameplay();

	auto update(float deltaTime) -> std::unique_ptr<State> override;
	void draw(engine::Renderer& renderer) const override;

  private:
	void transformCamera(float deltaTime);

	ChunkManager m_chunkManager{};
	engine::Camera m_camera{};
};
} // namespace resurgo