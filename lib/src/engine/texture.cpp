#include "resurgo/engine/texture.hpp"
#include "resurgo/utils/gl_handle.hpp"
#include <stb_image.h>
#include <stdexcept>

namespace resurgo::engine {
Texture::Texture(std::string const& path) : m_texture(createTexture()) {
	stbi_set_flip_vertically_on_load(true);

	auto* data = stbi_load(path.c_str(), &m_size[0], &m_size[1], &m_channels, 0);

	if (!data) { throw std::runtime_error{"Failed to load texture: " + path}; }

	auto format = GL_RGB;
	switch (m_channels) {
	case 1: format = GL_RED; break;
	case 3: format = GL_RGB; break;
	case 4: format = GL_RGBA; break;
	default:
		stbi_image_free(data);
		throw std::runtime_error{"Unsupported channel count: " + std::to_string(m_channels)};
	}

	glBindTexture(GL_TEXTURE_2D, m_texture.get());

	glTexImage2D(GL_TEXTURE_2D, 0, format, m_size[0], m_size[1], 0, static_cast<GLenum>(format), GL_UNSIGNED_BYTE,
				 data);

	glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	stbi_image_free(data);

	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::bind(unsigned int slot) const {
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, m_texture.get());
}

// NOLINTNEXTLINE(readability-convert-member-functions-to-static)
void Texture::unbind() const { glBindTexture(GL_TEXTURE_2D, 0); }
} // namespace resurgo::engine