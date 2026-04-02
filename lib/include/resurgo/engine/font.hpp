#pragma once
#include "resurgo/utils/gl_handle.hpp"
#include <ft2build.h>
#include <unordered_map>
#include FT_FREETYPE_H

#include <clib/base_types.hpp>
#include <glm/vec2.hpp>
#include <string>

namespace resurgo::engine {
struct Glyph {
	GLHandle texture{};
	glm::ivec2 size{};
	glm::ivec2 bearing{};
	int advance{};
};

// NOLINTNEXTLINE(cppcoreguidelines-special-member-functions)
class Font : public clib::MoveOnly {
  public:
	Font(std::string const& path, unsigned int charSize = 32);
	~Font();

	[[nodiscard]] auto getAscender(unsigned int charSize) -> int;
	[[nodiscard]] auto getDescender(unsigned int charSize) -> int;
	[[nodiscard]] auto getGlyph(unsigned char c, unsigned int charSize) -> Glyph const&;

  private:
	void updateSize(unsigned int charSize);

	FT_Library m_ft{};
	FT_Face m_face{};
	unsigned int m_charSize{};
	std::unordered_map<unsigned int, std::unordered_map<unsigned char, Glyph>> m_glyphs{};
};
} // namespace resurgo::engine