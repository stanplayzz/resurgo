#include "resurgo/engine/dynamic_geometry.hpp"

namespace resurgo::engine {
DynamicGeometry::DynamicGeometry() : m_vao(createVertexArray()), m_vbo(createBuffer()) {
	glBindVertexArray(m_vao.get());
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo.get());

	// NOLINTNEXTLINE(cppcoreguidelines-pro-type-cstyle-cast, performance-no-int-to-ptr)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
	glEnableVertexAttribArray(0);

	// NOLINTNEXTLINE(cppcoreguidelines-pro-type-cstyle-cast, performance-no-int-to-ptr)
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	glEnableVertexAttribArray(1);

	// NOLINTNEXTLINE(cppcoreguidelines-pro-type-cstyle-cast, performance-no-int-to-ptr)
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));
	glEnableVertexAttribArray(2);

	// NOLINTNEXTLINE(cppcoreguidelines-pro-type-cstyle-cast, performance-no-int-to-ptr)
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
	glEnableVertexAttribArray(3);

	glBindVertexArray(0);
}

void DynamicGeometry::clear() {
	m_vertices.clear();
	m_needsUpload = true;
}

void DynamicGeometry::addVertex(Vertex const& vertex) {
	m_vertices.push_back(vertex);
	m_needsUpload = true;
}

void DynamicGeometry::addTriangle(Vertex const& v1, Vertex const& v2, Vertex const& v3) {
	m_vertices.push_back(v1);
	m_vertices.push_back(v2);
	m_vertices.push_back(v3);
	m_needsUpload = true;
}

void DynamicGeometry::addQuad(Vertex const& v1, Vertex const& v2, Vertex const& v3, Vertex const& v4) {
	addTriangle(v1, v2, v3);
	addTriangle(v1, v3, v4);
}

void DynamicGeometry::upload() {
	if (!m_needsUpload) { return; }

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo.get());
	glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(m_vertices.size() * sizeof(Vertex)), m_vertices.data(),
				 GL_DYNAMIC_DRAW);
	m_needsUpload = false;
}

void DynamicGeometry::draw() const {
	if (m_vertices.empty()) { return; }

	glBindVertexArray(m_vao.get());
	glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(m_vertices.size()));
}

} // namespace resurgo::engine