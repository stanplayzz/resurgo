#pragma once
#include "resurgo/engine/vertex.hpp"
#include "resurgo/utils/gl_handle.hpp"
#include <clib/base_types.hpp>
#include <glad/glad.h>
#include <vector>

namespace resurgo::engine {
class Geometry : public clib::MoveOnly {
  public:
	Geometry(std::vector<Vertex> const& vertices, std::vector<unsigned int> const& indices);

	void draw() const;

  private:
	GLHandle m_vao{};
	GLHandle m_vbo{};
	GLHandle m_ebo{};
	int m_indexCount{};
};
} // namespace resurgo::engine