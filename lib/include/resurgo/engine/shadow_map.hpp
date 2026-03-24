#pragma once
#include "resurgo/engine/camera.hpp"
#include "resurgo/utils/gl_handle.hpp"
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

namespace resurgo::engine {
class ShadowMap {
  public:
	ShadowMap(int resolution = 1024);

	void bind() const {
		glViewport(0, 0, m_resolution, m_resolution);
		glBindFramebuffer(GL_FRAMEBUFFER, m_fbo.get());
		glClear(GL_DEPTH_BUFFER_BIT);
	}

	void bindDepthMap(int slot) {
		glActiveTexture(GL_TEXTURE0 + static_cast<GLenum>(slot));
		glBindTexture(GL_TEXTURE_2D, m_depthMap.get());
	}

	// NOLINTNEXTLINE(readability-convert-member-functions-to-static)
	void unbind(glm::ivec2 windowSize) const {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, windowSize.x, windowSize.y);
	}

	void updateLightSpaceMatrix(glm::vec3 const& lightDir, Camera const& camera);
	[[nodiscard]] auto getLightSpaceMatrix() const { return m_lightSpaceMatrix; }

  private:
	int m_resolution{};

	GLHandle m_fbo{};
	GLHandle m_depthMap{};

	glm::mat4 m_lightSpaceMatrix{};
};
} // namespace resurgo::engine