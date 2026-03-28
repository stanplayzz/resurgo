#pragma once
#include "resurgo/game/chunk_manager.hpp"
#include "resurgo/game/state_manager.hpp"

namespace resurgo {
class Gameplay : public State {
  public:
	Gameplay(engine::Camera& camera);

	auto update(float deltaTime, engine::Camera& camera) -> std::unique_ptr<State> override;
	void draw(engine::Renderer& renderer) const override;

  private:
	void transformCamera(float deltaTime, engine::Camera& camera);

	ChunkManager m_chunkManager{};
};
} // namespace resurgo