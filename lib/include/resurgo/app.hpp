#pragma once
#include <glad/glad.h>

#include "resurgo/engine/camera.hpp"
#include "resurgo/engine/renderer.hpp"
#include "resurgo/game/chunk_manager.hpp"
#include <GLFW/glfw3.h>

namespace resurgo {
// NOLINTNEXTLINE(cppcoreguidelines-special-member-functions)
class App { // ignore
  public:
	App();
	~App();

	void run();

  private:
	void transformCamera(float deltaTime);

	ChunkManager m_chunkManager{};

	GLFWwindow* m_window{};
	engine::Renderer m_renderer{};
	engine::Camera m_camera{};
};
} // namespace resurgo