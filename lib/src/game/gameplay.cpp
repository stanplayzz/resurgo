#include "resurgo/game/gameplay.hpp"
#include "resurgo/engine/input.hpp"

namespace resurgo {
namespace {
constexpr auto cameraSpeed_v = 200.f;
}

Gameplay::Gameplay(engine::Camera& camera) {
	// camera transform for dimetric view
	camera.transform.position.z = 700.f;
	camera.transform.rotation = {glm::radians(90.f) - std::atan(std::sin(glm::radians(30.f))), 0.f, glm::radians(45.f)};
}

auto Gameplay::update(float deltaTime, engine::Camera& camera) -> std::unique_ptr<State> {
	m_chunkManager.update({0, 0});
	transformCamera(deltaTime, camera);

	return nullptr;
}

void Gameplay::draw(engine::Renderer& renderer) const { m_chunkManager.draw(renderer); }

// NOLINTNEXTLINE(readability-convert-member-functions-to-static)
void Gameplay::transformCamera(float deltaTime, engine::Camera& camera) {
	if (engine::Input::isKeyPressed(GLFW_KEY_W)) { camera.transform.position.y += cameraSpeed_v * deltaTime; }
	if (engine::Input::isKeyPressed(GLFW_KEY_S)) { camera.transform.position.y -= cameraSpeed_v * deltaTime; }
	if (engine::Input::isKeyPressed(GLFW_KEY_A)) { camera.transform.position.x -= cameraSpeed_v * deltaTime; }
	if (engine::Input::isKeyPressed(GLFW_KEY_D)) { camera.transform.position.x += cameraSpeed_v * deltaTime; }
}

} // namespace resurgo