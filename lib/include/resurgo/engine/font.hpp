#pragma once
#include <ft2build.h>
#include FT_FREETYPE_H

#include <clib/base_types.hpp>
#include <glm/vec2.hpp>
#include <string>

namespace resurgo::engine {
// NOLINTNEXTLINE(cppcoreguidelines-special-member-functions)
class Font : public clib::MoveOnly {
  public:
	Font(std::string const& path, unsigned int charSize = 32);
	~Font();

	[[nodiscard]] auto getAscender() const -> int;
	[[nodiscard]] auto getDescender() const -> int;
	[[nodiscard]] auto getFace() const -> FT_Face { return m_face; }

  private:
	FT_Library m_ft{};
	FT_Face m_face{};
};
} // namespace resurgo::engine