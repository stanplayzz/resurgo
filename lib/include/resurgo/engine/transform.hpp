#pragma once
#include "glm/ext/matrix_transform.hpp"
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

namespace resurgo::engine {
class Transform {
  public:
	glm::vec3 position{0.0f};
	glm::vec3 rotation{0.0f};
	glm::vec3 scale{1.0f};

	[[nodiscard]] auto getModelMatrix() const -> glm::mat4 {
		auto model = glm::mat4{1.f};

		// translate
		model = glm::translate(model, position);

		// rotate
		model = glm::rotate(model, rotation.z, glm::vec3{0.f, 0.f, 1.f});
		model = glm::rotate(model, rotation.x, glm::vec3{1.f, 0.f, 0.f});
		model = glm::rotate(model, rotation.y, glm::vec3{0.f, 1.f, 0.f});

		// scale
		model = glm::scale(model, scale);

		return model;
	}
};
} // namespace resurgo::engine