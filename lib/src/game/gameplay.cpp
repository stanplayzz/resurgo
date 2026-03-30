#include "resurgo/game/gameplay.hpp"
#include "resurgo/engine/input.hpp"

namespace resurgo {
namespace {
constexpr auto cameraSpeed_v = 200.f;
}

Gameplay::Gameplay() {
	// camera transform for dimetric view
	m_camera.transform.position.z = 700.f;
	m_camera.transform.rotation = {glm::radians(90.f) - std::atan(std::sin(glm::radians(30.f))), 0.f,
								   glm::radians(45.f)};
	m_camera.updateSize({1280, 720});
}

auto Gameplay::update(float deltaTime) -> std::unique_ptr<State> {
	if (auto size = engine::Input::resized()) { m_camera.updateSize(*size); }

	m_chunkManager.update({0, 0});
	transformCamera(deltaTime);

	return nullptr;
}

void Gameplay::draw(engine::Renderer& renderer) const {
	renderer.setCamera(m_camera);
	m_chunkManager.draw(renderer);
}

// NOLINTNEXTLINE(readability-convert-member-functions-to-static)
void Gameplay::transformCamera(float deltaTime) {
	if (engine::Input::isKeyPressed(GLFW_KEY_W)) { m_camera.transform.position.y += cameraSpeed_v * deltaTime; }
	if (engine::Input::isKeyPressed(GLFW_KEY_S)) { m_camera.transform.position.y -= cameraSpeed_v * deltaTime; }
	if (engine::Input::isKeyPressed(GLFW_KEY_A)) { m_camera.transform.position.x -= cameraSpeed_v * deltaTime; }
	if (engine::Input::isKeyPressed(GLFW_KEY_D)) { m_camera.transform.position.x += cameraSpeed_v * deltaTime; }
}

} // namespace resurgo