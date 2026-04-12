#pragma once
#include <utility>

#include "resurgo/engine/game_object.hpp"

namespace resurgo::shape {
class Quad : public engine::GameObject {
  public:
	Quad(std::shared_ptr<engine::Shader> shader = engine::Resources::get().getDefaultShader())
		: engine::GameObject(createQuadGeometry(), std::move(shader)) {}

  private:
	static engine::Geometry createQuadGeometry() {
		std::vector<engine::Vertex> vertices = {
			{.position = {-0.5f, -0.5f, 0.0f}, .normal = {0.0f, 0.0f, 1.0f}, .texCoords = {0.0f, 0.0f}},
			{.position = {0.5f, -0.5f, 0.0f}, .normal = {0.0f, 0.0f, 1.0f}, .texCoords = {1.0f, 0.0f}},
			{.position = {0.5f, 0.5f, 0.0f}, .normal = {0.0f, 0.0f, 1.0f}, .texCoords = {1.0f, 1.0f}},
			{.position = {-0.5f, 0.5f, 0.0f}, .normal = {0.0f, 0.0f, 1.0f}, .texCoords = {0.0f, 1.0f}},
		};

		std::vector<unsigned int> indices = {0, 1, 2, 2, 3, 0};

		return {vertices, indices};
	}
};
} // namespace resurgo::shape