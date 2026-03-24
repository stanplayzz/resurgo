#pragma once
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

namespace resurgo::engine {
struct Vertex {
	glm::vec3 position{};
	glm::vec3 normal{};
	glm::vec2 texCoords{};
	glm::vec3 color{1.f};
};
} // namespace resurgo::engine