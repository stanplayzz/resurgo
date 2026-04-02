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

void Renderer::end(ShadowMap const* shadowMap) {
	if (!m_commands.empty()) { mainPass(shadowMap); }
	if (!m_textCommands.empty()) { textPass(); }
}

void Renderer::mainPass(ShadowMap const* shadowMap) {
	if (shadowMap) { shadowMap->bindDepthMap(1); }

	for (auto const& cmd : m_commands) {
		cmd.material->bind();
		auto& shader = cmd.material->getShader();
		shader.setUniform("u_Model", cmd.modelMatrix);
		shader.setUniform("u_ViewProjection", cmd.viewProjection);
		shader.setUniform("u_LightDir", lightDir_v);
		if (shadowMap) {
			shader.setUniform("u_LightSpaceMatrix", shadowMap->getLightSpaceMatrix());
			shader.setUniform("u_ShadowMap", 1);
		}

		cmd.geometry->draw();
	}
}

void Renderer::textPass() {
	glDisable(GL_DEPTH_TEST);
	for (auto const& cmd : m_textCommands) { cmd.text->draw(glm::ortho(0.f, cmd.cameraSize.x, cmd.cameraSize.y, 0.f)); }
	glEnable(GL_DEPTH_TEST);
}

// Shadow pass
// if (m_camera) { m_shadowMap->updateLightSpaceMatrix(lightDir_v, *m_camera); }
// m_shadowMap->bind();
// auto shader = Resources::instance().getShader("DepthShader");
// shader->use();

// for (auto const& cmd : m_commands) {
// 	shader->setUniform("u_Model", cmd.modelMatrix);
// 	shader->setUniform("u_LightSpaceMatrix", m_shadowMap->getLightSpaceMatrix());
// 	cmd.geometry->draw();
// }
// m_shadowMap->unbind(screenSize);

// // main pass
// m_shadowMap->bindDepthMap(1);
// for (auto const& cmd : m_commands) {
// 	cmd.material->bind();

// 	auto& shader = cmd.material->getShader();
// 	shader.setUniform("u_Model", cmd.modelMatrix);
// 	shader.setUniform("u_ViewProjection", m_viewProjection);
// 	shader.setUniform("u_LightDir", lightDir_v);
// 	shader.setUniform("u_LightSpaceMatrix", m_shadowMap->getLightSpaceMatrix());
// 	shader.setUniform("u_ShadowMap", 1);

// 	cmd.geometry->draw();
// }

// // text pass
// if (!m_texts.empty()) {
// 	auto proj = glm::ortho(0.f, m_camera->getSize().x, 0.f, m_camera->getSize().y);

// 	glDisable(GL_DEPTH_TEST);
// 	for (auto const* text : m_texts) { text->draw(proj); }
// 	glEnable(GL_DEPTH_TEST);
// }

} // namespace resurgo::engine