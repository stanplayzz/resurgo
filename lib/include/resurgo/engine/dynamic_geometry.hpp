#pragma once
#include "resurgo/engine/geometry.hpp"
#include "resurgo/engine/vertex.hpp"
#include "resurgo/utils/gl_handle.hpp"
#include <vector>

namespace resurgo::engine {
class DynamicGeometry : public IGeometry {
  public:
	DynamicGeometry();

	void clear();
	void addVertex(Vertex const& vertex);
	void addTriangle(Vertex const& v1, Vertex const& v2, Vertex const& v3);
	void addQuad(Vertex const& v1, Vertex const& v2, Vertex const& v3, Vertex const& v4);

	// Call this after changing geometry
	void upload();

	void draw() const override;

  private:
	GLHandle m_vao{};
	GLHandle m_vbo{};
	std::vector<Vertex> m_vertices{};
	bool m_needsUpload{};
};
} // namespace resurgo::engine