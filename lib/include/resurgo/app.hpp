#pragma once
#include <glad/glad.h>

#include "resurgo/engine/renderer.hpp"
#include "resurgo/game/state_manager.hpp"
#include <GLFW/glfw3.h>

namespace resurgo {
// NOLINTNEXTLINE(cppcoreguidelines-special-member-functions)
class App {
  public:
	App();
	~App();

	void run();

  private:
	void preloadResources();

	StateManager m_stateManager{};

	GLFWwindow* m_window{};
	glm::ivec2 m_windowSize{1280, 720};
	engine::Renderer m_renderer{};
	engine::Camera m_camera{};
};
} // namespace resurgo