#pragma once
#include "resurgo/engine/vertex.hpp"
#include "resurgo/utils/gl_handle.hpp"
#include <clib/base_types.hpp>
#include <vector>

namespace resurgo::engine {
class DynamicGeometry : public clib::MoveOnly {
  public:
	DynamicGeometry();

	void clear();
	void addVertex(Vertex const& vertex);
	void addTriangle(Vertex const& v1, Vertex const& v2, Vertex const& v3);
	void addQuad(Vertex const& v1, Vertex const& v2, Vertex const& v3, Vertex const& v4);

	// Call this after changing geometry
	void upload();

	void draw() const;

  private:
	GLHandle m_vao{};
	GLHandle m_vbo{};
	std::vector<Vertex> m_vertices{};
	bool m_needsUpload{};
};
} // namespace resurgo::engine