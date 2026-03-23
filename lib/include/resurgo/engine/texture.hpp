#pragma once
#include "resurgo/utils/gl_handle.hpp"
#include <glm/vec2.hpp>
#include <string>

namespace resurgo::engine {
class Texture {
  public:
	explicit Texture(std::string const& path);

	void bind(unsigned int slot = 0) const;
	void unbind() const;

	[[nodiscard]] auto getSize() const -> glm::ivec2 { return m_size; }
	[[nodiscard]] auto getId() const -> unsigned int { return m_texture.get(); }

  private:
	GLHandle m_texture{};
	int m_channels{};
	glm::ivec2 m_size{};
};
} // namespace resurgo::engine