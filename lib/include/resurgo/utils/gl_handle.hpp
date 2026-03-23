#pragma once
#include <glad/glad.h>
#include <cstdint>

namespace resurgo::engine {

enum class GLObjectType : std::int8_t { VertexArray, Buffer, Texture, Shader, Program, Framebuffer, Renderbuffer };

class GLHandle {
  public:
	GLHandle() = default;

	GLHandle(GLObjectType type, GLuint id) : m_type(type), m_id(id) {}

	~GLHandle() {
		if (m_id != 0) { destroy(); }
	}

	// Delete copy
	GLHandle(GLHandle const&) = delete;
	GLHandle& operator=(GLHandle const&) = delete;

	// Allow move
	GLHandle(GLHandle&& other) noexcept : m_type(other.m_type), m_id(other.m_id) { other.m_id = 0; }

	GLHandle& operator=(GLHandle&& other) noexcept {
		if (this != &other) {
			if (m_id != 0) { destroy(); }
			m_type = other.m_type;
			m_id = other.m_id;
			other.m_id = 0;
		}
		return *this;
	}

	[[nodiscard]] GLuint get() const { return m_id; }

	GLuint release() {
		GLuint id = m_id;
		m_id = 0;
		return id;
	}

	explicit operator bool() const { return m_id != 0; }

  private:
	void destroy() {
		switch (m_type) {
		case GLObjectType::VertexArray: glDeleteVertexArrays(1, &m_id); break;
		case GLObjectType::Buffer: glDeleteBuffers(1, &m_id); break;
		case GLObjectType::Texture: glDeleteTextures(1, &m_id); break;
		case GLObjectType::Shader: glDeleteShader(m_id); break;
		case GLObjectType::Program: glDeleteProgram(m_id); break;
		case GLObjectType::Framebuffer: glDeleteFramebuffers(1, &m_id); break;
		case GLObjectType::Renderbuffer: glDeleteRenderbuffers(1, &m_id); break;
		}
	}

	GLObjectType m_type{};
	GLuint m_id = 0;
};

// Factory functions
inline GLHandle createVertexArray() {
	GLuint id{};
	glGenVertexArrays(1, &id);
	return {GLObjectType::VertexArray, id};
}

inline GLHandle createBuffer() {
	GLuint id{};
	glGenBuffers(1, &id);
	return {GLObjectType::Buffer, id};
}

inline GLHandle createTexture() {
	GLuint id{};
	glGenTextures(1, &id);
	return {GLObjectType::Texture, id};
}

inline GLHandle createProgram() { return GLHandle(GLObjectType::Program, glCreateProgram()); }

inline GLHandle createShader(GLenum type) { return GLHandle(GLObjectType::Shader, glCreateShader(type)); }

inline GLHandle createFramebuffer() {
	GLuint id{};
	glGenFramebuffers(1, &id);
	return {GLObjectType::Framebuffer, id};
}

inline GLHandle createRenderbuffer() {
	GLuint id{};
	glGenRenderbuffers(1, &id);
	return {GLObjectType::Renderbuffer, id};
}

} // namespace resurgo::engine