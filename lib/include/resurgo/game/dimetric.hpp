#pragma once
#include "glm/trigonometric.hpp"
#include <glm/vec3.hpp>

namespace resurgo {
auto const dimetricView_v =
	glm::vec3{glm::radians(90.f) - std::atan(std::sin(glm::radians(30.f))), 0.f, glm::radians(45.f)};
}