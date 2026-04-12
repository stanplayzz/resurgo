#include "resurgo/engine/renderer.hpp"

namespace resurgo::engine {
namespace {
constexpr auto lightDir_v = glm::vec3{0.2f, -0.6f, 1.f};
}

Renderer::Renderer() {
	Camera defaultCam{};
	m_activeViewProjection = defaultCam.getProjectionMatrix() * defaultCam.getViewMatrix();
	m_activeCameraSize = defaultCam.getSize();
}

void Renderer::begin() {
	m_commands.clear();
	m_textCommands.clear();
}

void Renderer::setCamera(Camera const& camera) {
	m_activeViewProjection = camera.getProjectionMatrix() * camera.getViewMatrix();
	m_activeCameraSize = camera.getSize();
}

void Renderer::draw(IGeometry const* geometry, Material const* material, Transform transform) {
	m_commands.push_back({
		.geometry = geometry,
		.material = material,
		.modelMatrix = transform.getModelMatrix(),
		.viewProjection = m_activeViewProjection,
	});
}

void Renderer::draw(Text const& text) { m_textCommands.push_back({.text = &text, .cameraSize = m_activeCameraSize}); }

void Renderer::end() {
	if (!m_commands.empty()) { mainPass(); }
	if (!m_textCommands.empty()) { textPass(); }
}

void Renderer::mainPass() {
	for (auto const& cmd : m_commands) {
		cmd.material->bind();
		auto& shader = cmd.material->getShader();
		shader.setUniform("u_Model", cmd.modelMatrix);
		shader.setUniform("u_ViewProjection", cmd.viewProjection);
		shader.setUniform("u_LightDir", lightDir_v);

		cmd.geometry->draw();
	}
}

void Renderer::textPass() {
	glDisable(GL_DEPTH_TEST);
	for (auto const& cmd : m_textCommands) { cmd.text->draw(glm::ortho(0.f, cmd.cameraSize.x, cmd.cameraSize.y, 0.f)); }
	glEnable(GL_DEPTH_TEST);
}
} // namespace resurgo::engine