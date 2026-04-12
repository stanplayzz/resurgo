#include "resurgo/engine/text.hpp"
#include "resurgo/engine/resources.hpp"
#include <ft2build.h>
#include FT_FREETYPE_H

#include <utility>

namespace resurgo::engine {
Text::Text(Font& font) : m_font(font), m_vao(createVertexArray()), m_vbo(createBuffer()) {
	glBindVertexArray(m_vao.get());
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo.get());
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, nullptr, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), nullptr);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	m_shader = Resources::get().loadShader("TextShader", "shaders/text.vert", "shaders/text.frag").get();
}

void Text::setCharacterSize(unsigned int charSize) { m_charSize = charSize; }

void Text::setString(std::string string) { m_string = std::move(string); }
void Text::setColor(Color color) { m_color = color; }
void Text::setPosition(glm::vec2 position) { m_position = position; }

auto Text::getBounds() const -> FloatRect {
	auto x = 0.f;
	auto minY = std::numeric_limits<float>::max();
	auto maxY = std::numeric_limits<float>::lowest();

	if (m_string.empty()) { return {}; }

	for (auto const& c : m_string) {
		auto const& ch = m_font.getGlyph(static_cast<unsigned char>(c), m_charSize);

		auto yPos = static_cast<float>(ch.size.y - ch.bearing.y);
		auto h = static_cast<float>(ch.size.y);

		minY = std::min(minY, yPos);
		maxY = std::max(maxY, yPos + h);

		x += static_cast<float>(ch.advance >> 6);
	}

	return {.size = {x, maxY - minY}, .position = m_position + glm::vec2{0.f, minY}};
}

void Text::draw(glm::mat4 proj) const {
	m_shader->use();
	m_shader->setUniform("u_TextColor", m_color.toVec3());
	m_shader->setUniform("u_Projection", proj);
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(m_vao.get());

	auto x = m_position.x;
	for (auto const& c : m_string) {
		auto const& ch = m_font.getGlyph(static_cast<unsigned char>(c), m_charSize);

		auto xPos = x + static_cast<float>(ch.bearing.x);
		auto yPos = m_position.y + static_cast<float>(ch.size.y - ch.bearing.y);
		auto w = static_cast<float>(ch.size.x);
		auto h = static_cast<float>(ch.size.y);

		auto vertices = std::array<std::array<float, 4>, 6>{{
			{xPos, yPos, 0, 0},
			{xPos, yPos + h, 0, 1},
			{xPos + w, yPos + h, 1, 1},
			{xPos, yPos, 0, 0},
			{xPos + w, yPos + h, 1, 1},
			{xPos + w, yPos, 1, 0},
		}};

		glBindTexture(GL_TEXTURE_2D, ch.texture.get());
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo.get());
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices.data()->data());
		glDrawArrays(GL_TRIANGLES, 0, 6);
		x += static_cast<float>(ch.advance >> 6);
	}

	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}
} // namespace resurgo::engine