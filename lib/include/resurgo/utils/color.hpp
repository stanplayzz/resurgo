#pragma once
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

namespace resurgo {
struct Color {
	float r{1.f};
	float g{1.f};
	float b{1.f};
	float a{1.f};

	[[nodiscard]] auto toVec3() const -> glm::vec3 { return {r, g, b}; }
	[[nodiscard]] auto toVec4() const -> glm::vec4 { return {r, g, b, a}; }

	static Color const Red;
	static Color const Green;
	static Color const Blue;
	static Color const White;
	static Color const Black;
	static Color const Transparent;
};

inline constexpr Color Color::Red{.r = 1.f, .g = 0.f, .b = 0.f, .a = 1.f};
inline constexpr Color Color::Green{.r = 0.f, .g = 1.f, .b = 0.f, .a = 1.f};
inline constexpr Color Color::Blue{.r = 0.f, .g = 0.f, .b = 1.f, .a = 1.f};
inline constexpr Color Color::White{.r = 1.f, .g = 1.f, .b = 1.f, .a = 1.f};
inline constexpr Color Color::Black{.r = 0.f, .g = 0.f, .b = 0.f, .a = 1.f};
inline constexpr Color Color::Transparent{.r = 0.f, .g = 0.f, .b = 0.f, .a = 0.f};

} // namespace resurgo