#pragma once
#include "resurgo/engine/camera.hpp"
#include "resurgo/engine/geometry.hpp"
#include "resurgo/engine/material.hpp"
#include "resurgo/engine/text.hpp"
#include "resurgo/utils/color.hpp"
#include <GL/gl.h>
#include <glm/mat4x4.hpp>

namespace resurgo::engine {
class Renderer {
  public:
	Renderer();

	void begin();
	void draw(IGeometry const* geometry, Material const* material, Transform transform);
	void draw(Text const& text);
	void end();

	void setCamera(Camera const& camera);

	// NOLINTNEXTLINE(readability-convert-member-functions-to-static)
	void clear(Color const& c = {}) {
		glClearColor(c.r, c.g, c.b, c.a);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

  private:
	void mainPass();
	void textPass();

	struct RenderCommand {
		IGeometry const* geometry;
		Material const* material;
		glm::mat4 modelMatrix;
		glm::mat4 viewProjection;
	};

	struct TextCommand {
		Text const* text;
		glm::vec2 cameraSize;
	};

	std::vector<RenderCommand> m_commands{};
	std::vector<TextCommand> m_textCommands{};

	glm::mat4 m_activeViewProjection{};
	glm::vec2 m_activeCameraSize{};
};
} // namespace resurgo::engine