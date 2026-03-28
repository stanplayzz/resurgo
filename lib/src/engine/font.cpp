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

auto Font::getAscender() const -> int { return static_cast<int>(m_face->size->metrics.ascender >> 6); }
auto Font::getDescender() const -> int { return static_cast<int>(m_face->size->metrics.descender >> 6); }
} // namespace resurgo::engine