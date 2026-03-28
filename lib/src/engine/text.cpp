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
	update();
}

void Text::setString(std::string string) { m_string = std::move(string); }
void Text::setColor(Color color) { m_color = color; }
void Text::setPosition(glm::vec2 position) { m_position = position; }

void Text::draw(glm::mat4 proj) const {
	auto shader = Resources::instance().loadShader("TextShader", ASSETS_DIR "/shaders/text.vert",
												   ASSETS_DIR "/shaders/text.frag");

	shader->use();
	shader->setUniform("u_TextColor", m_color.toVec3());
	shader->setUniform("u_Projection", proj);
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(m_vao.get());

	auto x = m_position.x;
	for (auto const& c : m_string) {
		auto const& ch = m_characters.at(c);

		auto xPos = x + static_cast<float>(ch.bearing.x);
		auto yPos = m_position.y - static_cast<float>(ch.size.y - ch.bearing.y);
		auto w = static_cast<float>(ch.size.x);
		auto h = static_cast<float>(ch.size.y);

		auto vertices = std::array<std::array<float, 4>, 6>{{
			{xPos, yPos + h, 0, 0},
			{xPos, yPos, 0, 1},
			{xPos + w, yPos, 1, 1},
			{xPos, yPos + h, 0, 0},
			{xPos + w, yPos, 1, 1},
			{xPos + w, yPos + h, 1, 0},
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

void Text::update() {
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	auto* glyph = m_font.getFace()->glyph;

	for (unsigned char c = 0; c < 128; c++) {
		if (FT_Load_Char(m_font.getFace(), c, FT_LOAD_RENDER)) { throw std::runtime_error{"Failed to load glyph"}; }

		auto texture = createTexture();
		glBindTexture(GL_TEXTURE_2D, texture.get());
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, static_cast<GLsizei>(glyph->bitmap.width),
					 static_cast<GLsizei>(glyph->bitmap.rows), 0, GL_RED, GL_UNSIGNED_BYTE,
					 m_font.getFace()->glyph->bitmap.buffer);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		auto character =
			Character{.texture = std::move(texture),
					  .size = {glyph->bitmap.width, glyph->bitmap.rows},
					  .bearing = {m_font.getFace()->glyph->bitmap_left, m_font.getFace()->glyph->bitmap_top},
					  .advance = static_cast<int>(m_font.getFace()->glyph->advance.x)};
		m_characters.emplace(c, std::move(character));
	}
}

} // namespace resurgo::engine