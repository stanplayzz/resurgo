#pragma once
#include "resurgo/engine/font.hpp"
#include "resurgo/utils/color.hpp"
#include "resurgo/utils/gl_handle.hpp"
#include <glm/mat4x4.hpp>
#include <map>

namespace resurgo::engine {
struct Character {
	GLHandle texture{};
	glm::ivec2 size{};
	glm::ivec2 bearing{};
	int advance{};
};

class Text {
  public:
	Text(Font& font);

	void setString(std::string string);
	void setCharacterSize(unsigned int charSize);
	void setColor(Color color);
	void setPosition(glm::vec2 position);

  private:
	void update();

	void draw(glm::mat4 proj) const;
	friend class Renderer;

	std::map<char, Character> m_characters{};
	Font& m_font;
	GLHandle m_vao{};
	GLHandle m_vbo{};

	std::string m_string{};
	Color m_color{};
	glm::vec2 m_position{};
};
} // namespace resurgo::engine