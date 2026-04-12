#include "resurgo/game/gameplay.hpp"
#include "resurgo/engine/input.hpp"
#include "resurgo/game/dimetric.hpp"

namespace resurgo {
Gameplay::Gameplay() {
	// camera transform for dimetric view
	m_camera.transform.rotation = dimetricView_v;
	m_camera.updateSize({1280, 720});
	m_camera.transform.position.z = 700.f;

	// preload
	engine::Resources::get().loadShader("TerrainShader", "shaders/terrain.vert", "shaders/terrain.frag");
}

auto Gameplay::update(float deltaTime) -> std::unique_ptr<State> {
	if (auto size = engine::Input::resized()) { m_camera.updateSize(*size); }

	m_chunkManager.update(m_player.getPosition());
	m_player.update(deltaTime, m_chunkManager);
	transformCamera(deltaTime);

	m_cursor.update(deltaTime, m_camera);

	return nullptr;
}

void Gameplay::draw(engine::Renderer& renderer) const {
	renderer.setCamera(m_camera);
	m_chunkManager.draw(renderer);
	m_player.draw(renderer);
	m_cursor.draw(renderer);
}

// NOLINTNEXTLINE(readability-convert-member-functions-to-static)
void Gameplay::transformCamera(float deltaTime) {
	glm::vec3 target{};
	target.x = m_player.getPosition().x + 494.f;
	target.y = m_player.getPosition().y - 494.f;
	target.z = 700.f;
	m_camera.transform.position += (target - m_camera.transform.position) * 5.f * deltaTime;
}

} // namespace resurgo