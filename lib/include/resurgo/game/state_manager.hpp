#pragma once
#include "resurgo/engine/renderer.hpp"
#include <clib/base_types.hpp>

namespace resurgo {
class State : public clib::Polymorphic {
  public:
	virtual auto update(float deltaTime) -> std::unique_ptr<State> = 0;
	virtual void draw(engine::Renderer& renderer) const = 0;
};

class StateManager {
  public:
	void switchState(std::unique_ptr<State> newState) { m_currentState = std::move(newState); }

	void update(float deltaTime) {
		if (m_currentState) {
			if (auto nextState = m_currentState->update(deltaTime)) { m_currentState = std::move(nextState); }
		}
	}
	void draw(engine::Renderer& renderer) const {
		if (m_currentState) { m_currentState->draw(renderer); }
	}

  private:
	std::unique_ptr<State> m_currentState{};
};
} // namespace resurgo