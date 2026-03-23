#include "resurgo/engine/renderer.hpp"
#include "resurgo/engine/game_object.hpp"

namespace resurgo::engine {
void Renderer::begin(Camera const& camera) {
	m_viewProjection = camera.getProjectionMatrix() * camera.getViewMatrix();
	m_commands.clear();
}

void Renderer::draw(GameObject const& gameObject) {
	RenderCommand cmd{};
	// cmd.geometry = &gameObject.getGeometry();
	cmd.material = &gameObject.material;
	cmd.modelMatrix = gameObject.transform.getModelMatrix();

	m_commands.push_back(cmd);
}

void Renderer::draw(DynamicGeometry const* geometry, Material const* material, Transform transform) {
	RenderCommand cmd{};
	cmd.geometry = geometry;
	cmd.material = material;
	cmd.modelMatrix = transform.getModelMatrix();

	m_commands.push_back(cmd);
}

void Renderer::end() {
	for (auto const& cmd : m_commands) {
		cmd.material->bind();

		auto& shader = cmd.material->getShader();
		shader.setUniform("u_Model", cmd.modelMatrix);
		shader.setUniform("u_ViewProjection", m_viewProjection);

		cmd.geometry->draw();
	}
}

} // namespace resurgo::engine