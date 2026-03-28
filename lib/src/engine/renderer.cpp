#include "resurgo/engine/renderer.hpp"

namespace resurgo::engine {
void Renderer::begin(Camera const& camera) {
	if (!m_shadowMap) { m_shadowMap.emplace(4096); }

	m_viewProjection = camera.getProjectionMatrix() * camera.getViewMatrix();
	m_camera = &camera;
	m_commands.clear();
	m_texts.clear();
}

void Renderer::draw(IGeometry const* geometry, Material const* material, Transform transform) {
	RenderCommand cmd{};
	cmd.geometry = geometry;
	cmd.material = material;
	cmd.modelMatrix = transform.getModelMatrix();

	m_commands.push_back(cmd);
}

void Renderer::draw(Text& text) { m_texts.push_back(&text); }

void Renderer::end(glm::ivec2 screenSize) {
	// Shadow pass
	if (m_camera) { m_shadowMap->updateLightSpaceMatrix(m_lightDir, *m_camera); }
	m_shadowMap->bind();
	auto shader = Resources::instance().getShader("DepthShader");
	shader->use();

	for (auto const& cmd : m_commands) {
		shader->setUniform("u_Model", cmd.modelMatrix);
		shader->setUniform("u_LightSpaceMatrix", m_shadowMap->getLightSpaceMatrix());
		cmd.geometry->draw();
	}
	m_shadowMap->unbind(screenSize);

	// main pass
	m_shadowMap->bindDepthMap(1);
	for (auto const& cmd : m_commands) {
		cmd.material->bind();

		auto& shader = cmd.material->getShader();
		shader.setUniform("u_Model", cmd.modelMatrix);
		shader.setUniform("u_ViewProjection", m_viewProjection);
		shader.setUniform("u_LightDir", m_lightDir);
		shader.setUniform("u_LightSpaceMatrix", m_shadowMap->getLightSpaceMatrix());
		shader.setUniform("u_ShadowMap", 1);

		cmd.geometry->draw();
	}

	// text pass
	if (!m_texts.empty()) {
		auto proj = glm::ortho(0.f, m_camera->getSize().x, 0.f, m_camera->getSize().y);

		glDisable(GL_DEPTH_TEST);
		for (auto const* text : m_texts) { text->draw(proj); }
		glEnable(GL_DEPTH_TEST);
	}
}

} // namespace resurgo::engine