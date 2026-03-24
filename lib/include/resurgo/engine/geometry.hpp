#pragma once
#include "resurgo/engine/vertex.hpp"
#include "resurgo/utils/gl_handle.hpp"
#include <clib/base_types.hpp>
#include <glad/glad.h>
#include <vector>

namespace resurgo::engine {
class IGeometry : public clib::Polymorphic, public clib::MoveOnly {
  public:
	virtual void draw() const = 0;
};

class Geometry : public IGeometry {
  public:
	Geometry(std::vector<Vertex> const& vertices, std::vector<unsigned int> const& indices);

	void draw() const override;

  private:
	GLHandle m_vao{};
	GLHandle m_vbo{};
	GLHandle m_ebo{};
	int m_indexCount{};
};
} // namespace resurgo::engine