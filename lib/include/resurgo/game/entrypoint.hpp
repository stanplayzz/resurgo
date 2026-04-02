#pragma once
#include "resurgo/engine/shapes/quad.hpp"
#include "resurgo/game/state_manager.hpp"

namespace resurgo {
class Entrypoint : public State {
  public:
	Entrypoint();
	auto update(float deltaTime) -> std::unique_ptr<State> override;
	void draw(engine::Renderer& renderer) const override;

  private:
	engine::Text m_title;

	engine::Text m_playButton;

	shape::Quad m_background{};
	shape::Quad m_planet{};

	engine::Camera m_camera{};
};
} // namespace resurgo