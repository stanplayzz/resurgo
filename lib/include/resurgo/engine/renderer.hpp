#pragma once
#include "resurgo/engine/camera.hpp"
#include "resurgo/engine/geometry.hpp"
#include "resurgo/engine/material.hpp"
#include "resurgo/engine/shadow_map.hpp"
#include "resurgo/engine/text.hpp"
#include "resurgo/utils/color.hpp"
#include <GL/gl.h>
#include <glm/mat4x4.hpp>
#include <optional>

namespace resurgo::engine {
class Renderer {
  public:
	void begin(Camera const& camera);
	void draw(IGeometry const* geometry, Material const* material, Transform transform);
	void draw(Text& text);
	void end(glm::ivec2 screenSize);

	// NOLINTNEXTLINE(readability-convert-member-functions-to-static)
	void clear(Color const& c = {}) {
		glClearColor(c.r, c.g, c.b, c.a);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

  private:
	struct RenderCommand {
		IGeometry const* geometry;
		Material const* material;
		glm::mat4 modelMatrix;
	};

	glm::mat4 m_viewProjection{};
	std::vector<RenderCommand> m_commands{};
	std::vector<Text const*> m_texts{};
	Camera const* m_camera{};

	std::optional<ShadowMap> m_shadowMap{};
	glm::vec3 m_lightDir{0.2f, -0.6f, 1.f};
};
} // namespace resurgo::engine