#pragma once
#include "resurgo/engine/transform.hpp"
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

namespace resurgo::engine {
class Camera {
  public:
	Transform transform{};

	float nearPlane = 0.1f;
	float farPlane = 10000.0f;

	[[nodiscard]] auto getViewMatrix() const -> glm::mat4 { return glm::inverse(transform.getModelMatrix()); }

	[[nodiscard]] auto getProjectionMatrix() const -> glm::mat4 {
		auto height = m_size.y;
		auto width = m_size.x;
		return glm::ortho(-width * 0.5f, width * 0.5f, -height * 0.5f, height * 0.5f, nearPlane, farPlane);
	}

	void updateSize(glm::vec2 windowSize) { m_size = windowSize; }

  private:
	glm::vec2 m_size{};
};
} // namespace resurgo::engine