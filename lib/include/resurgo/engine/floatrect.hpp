#pragma once
#include <glm/vec2.hpp>

namespace resurgo::engine {
struct FloatRect {
	glm::vec2 size{};
	glm::vec2 position{};

	[[nodiscard]] auto contains(glm::vec2 point) const -> bool {
		return (point.x >= position.x && point.y >= position.y && point.x <= position.x + size.x &&
				point.y <= position.y + size.y);
	}
};
} // namespace resurgo::engine