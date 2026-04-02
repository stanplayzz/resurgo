#include "resurgo/engine/font.hpp"
#include <GL/gl.h>
#include <stdexcept>

namespace resurgo::engine {
Font::Font(std::string const& path, unsigned int charSize) {
	if (FT_Init_FreeType(&m_ft)) { throw std::runtime_error{"Failed to initialize FreeType"}; }
	if (FT_New_Face(m_ft, path.c_str(), 0, &m_face)) { throw std::runtime_error{"Failed to load font: " + path}; }
	FT_Set_Pixel_Sizes(m_face, 0, charSize);
}

Font::~Font() {
	FT_Done_Face(m_face);
	FT_Done_FreeType(m_ft);
}

auto Font::getAscender(unsigned int charSize) -> int {
	updateSize(charSize);
	return static_cast<int>(m_face->size->metrics.ascender >> 6);
}
auto Font::getDescender(unsigned int charSize) -> int {
	updateSize(charSize);
	return static_cast<int>(m_face->size->metrics.descender >> 6);
}

auto Font::getGlyph(unsigned char c, unsigned int charSize) -> Glyph const& {
	auto& sizeCache = m_glyphs[charSize];
	if (auto it = sizeCache.find(c); it != sizeCache.end()) { return it->second; }

	updateSize(charSize);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	if (FT_Load_Char(m_face, c, FT_LOAD_RENDER)) { throw std::runtime_error{"Failed to load glyph"}; }

	auto* glyph = m_face->glyph;
	auto texture = createTexture();
	glBindTexture(GL_TEXTURE_2D, texture.get());
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, static_cast<GLsizei>(glyph->bitmap.width),
				 static_cast<GLsizei>(glyph->bitmap.rows), 0, GL_RED, GL_UNSIGNED_BYTE, glyph->bitmap.buffer);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	return sizeCache
		.emplace(c,
				 Glyph{
					 .texture = std::move(texture),
					 .size = {glyph->bitmap.width, glyph->bitmap.rows},
					 .bearing = {glyph->bitmap_left, glyph->bitmap_top},
					 .advance = static_cast<int>(glyph->advance.x),
				 })
		.first->second;
}

void Font::updateSize(unsigned int charSize) {
	if (m_charSize == charSize) { return; }
	FT_Set_Pixel_Sizes(m_face, 0, charSize);
	m_charSize = charSize;
}
} // namespace resurgo::engine