#pragma once
#include "resurgo/engine/floatrect.hpp"
#include "resurgo/engine/font.hpp"
#include "resurgo/engine/shader.hpp"
#include "resurgo/utils/color.hpp"
#include "resurgo/utils/gl_handle.hpp"
#include <glm/mat4x4.hpp>

namespace resurgo::engine {
class Text {
  public:
	Text(Font& font);

	void setString(std::string string);
	void setCharacterSize(unsigned int charSize);
	void setColor(Color color);
	void setPosition(glm::vec2 position);

	[[nodiscard]] auto getBounds() const -> FloatRect;
	[[nodiscard]] auto getPosition() const -> glm::vec2 { return m_position; }

  private:
	void draw(glm::mat4 proj) const;
	friend class Renderer;

	Font& m_font;
	unsigned int m_charSize{32};

	GLHandle m_vao{};
	GLHandle m_vbo{};
	Shader* m_shader{};

	std::string m_string{};
	Color m_color{};
	glm::vec2 m_position{};
};
} // namespace resurgo::engine