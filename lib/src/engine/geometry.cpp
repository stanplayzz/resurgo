#include "resurgo/engine/geometry.hpp"
#include "resurgo/utils/gl_handle.hpp"

namespace resurgo::engine {
Geometry::Geometry(std::vector<Vertex> const& vertices, std::vector<unsigned int> const& indices)
	: m_vao(createVertexArray()), m_vbo(createBuffer()), m_ebo(createBuffer()),
	  m_indexCount(static_cast<int>(indices.size())) {
	glBindVertexArray(m_vao.get());

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo.get());
	glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(vertices.size() * sizeof(Vertex)), vertices.data(),
				 GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo.get());
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizeiptr>(indices.size() * sizeof(unsigned int)),
				 indices.data(), GL_STATIC_DRAW);

	// NOLINTNEXTLINE(cppcoreguidelines-pro-type-cstyle-cast, performance-no-int-to-ptr)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
	glEnableVertexAttribArray(0);

	// NOLINTNEXTLINE(cppcoreguidelines-pro-type-cstyle-cast, performance-no-int-to-ptr)
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	glEnableVertexAttribArray(1);

	// NOLINTNEXTLINE(cppcoreguidelines-pro-type-cstyle-cast, performance-no-int-to-ptr)
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);
}

void Geometry::draw() const {
	glBindVertexArray(m_vao.get());
	glDrawElements(GL_TRIANGLES, m_indexCount, GL_UNSIGNED_INT, nullptr);
	glBindVertexArray(0);
}
} // namespace resurgo::engine