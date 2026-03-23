#pragma once
#include "resurgo/engine/game_object.hpp"
#include "resurgo/engine/resources.hpp"
#include <memory>

namespace resurgo::shape {
class Cube : public engine::GameObject {
  public:
	Cube(std::shared_ptr<engine::Shader> shader = engine::Resources::instance().getDefaultShader())
		: engine::GameObject(createGeometry(), std::move(shader)) {}

  private:
	static engine::Geometry createGeometry() {
		auto half = 1 * 0.5f;
		std::vector<engine::Vertex> vertices = {
			{{-half, -half, half}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}},
			{{half, -half, half}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}},
			{{half, half, half}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
			{{-half, half, half}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},
			{{half, -half, -half}, {0.0f, 0.0f, -1.0f}, {0.0f, 0.0f}},
			{{-half, -half, -half}, {0.0f, 0.0f, -1.0f}, {1.0f, 0.0f}},
			{{-half, half, -half}, {0.0f, 0.0f, -1.0f}, {1.0f, 1.0f}},
			{{half, half, -half}, {0.0f, 0.0f, -1.0f}, {0.0f, 1.0f}},
			{{half, -half, half}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
			{{half, -half, -half}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
			{{half, half, -half}, {1.0f, 0.0f, 0.0f}, {1.0f, 1.0f}},
			{{half, half, half}, {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f}},
			{{-half, -half, -half}, {-1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
			{{-half, -half, half}, {-1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
			{{-half, half, half}, {-1.0f, 0.0f, 0.0f}, {1.0f, 1.0f}},
			{{-half, half, -half}, {-1.0f, 0.0f, 0.0f}, {0.0f, 1.0f}},
			{{-half, half, half}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
			{{half, half, half}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
			{{half, half, -half}, {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f}},
			{{-half, half, -half}, {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f}},
			{{-half, -half, -half}, {0.0f, -1.0f, 0.0f}, {0.0f, 0.0f}},
			{{half, -half, -half}, {0.0f, -1.0f, 0.0f}, {1.0f, 0.0f}},
			{{half, -half, half}, {0.0f, -1.0f, 0.0f}, {1.0f, 1.0f}},
			{{-half, -half, half}, {0.0f, -1.0f, 0.0f}, {0.0f, 1.0f}},
		};

		std::vector<unsigned int> indices = {
			0,	1,	2,	2,	3,	0,	4,	5,	6,	6,	7,	4,	8,	9,	10, 10, 11, 8,
			12, 13, 14, 14, 15, 12, 16, 17, 18, 18, 19, 16, 20, 21, 22, 22, 23, 20,
		};

		return {vertices, indices};
	}
};
} // namespace resurgo::shape