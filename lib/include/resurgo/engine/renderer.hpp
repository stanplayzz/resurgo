#pragma once
#include "resurgo/engine/camera.hpp"
#include "resurgo/engine/dynamic_geometry.hpp"
#include "resurgo/engine/geometry.hpp"
#include "resurgo/engine/material.hpp"
#include "resurgo/utils/color.hpp"
#include <GL/gl.h>
#include <glm/mat4x4.hpp>

namespace resurgo::engine {
class GameObject;

class Renderer {
  public:
	void begin(Camera const& camera);
	void draw(GameObject const& gameObject);
	void draw(DynamicGeometry const* geometry, Material const* material, Transform transform);
	void end();

	// NOLINTNEXTLINE(readability-convert-member-functions-to-static)
	void clear(Color const& c = {}) {
		glClearColor(c.r, c.g, c.b, c.a);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

  private:
	struct RenderCommand {
		DynamicGeometry const* geometry;
		Material const* material;
		glm::mat4 modelMatrix;
	};

	glm::mat4 m_viewProjection{};
	std::vector<RenderCommand> m_commands{};
};
} // namespace resurgo::engine