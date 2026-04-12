#pragma once
#include "resurgo/engine/floatrect.hpp"
#include "resurgo/engine/resources.hpp"
#include "resurgo/utils/color.hpp"
#include <memory>
#include <optional>
#include <utility>

namespace resurgo::engine {
class Material {
  public:
	explicit Material(std::shared_ptr<Shader> shader = Resources::get().getDefaultShader())
		: m_shader(std::move(shader)) {}

	void setColor(Color const& color) { m_color = color; }
	void setTexture(std::shared_ptr<Texture> texture) { m_texture = std::move(texture); }
	void setTextureRect(FloatRect const& rect) { m_textureRect = rect; }

	void setFloat(std::string const& name, float v) { m_floats[name] = v; }
	void setVec2(std::string const& name, glm::vec2 v) { m_vec2s[name] = v; }
	void setVec3(std::string const& name, glm::vec3 v) { m_vec3s[name] = v; }
	void setVec4(std::string const& name, glm::vec4 v) { m_vec4s[name] = v; }
	void setInt(std::string const& name, int v) { m_ints[name] = v; }
	void setMat4(std::string const& name, glm::mat4 v) { m_mat4s[name] = v; }

	void bind() const {
		m_shader->use();
		m_shader->setUniform("u_Color", m_color.toVec4());

		if (m_texture) {
			m_shader->setUniform("u_UseTexture", true);
			m_texture->bind(0);
			m_shader->setUniform("u_Texture", 0);

			auto texSize = glm::vec2(m_texture->getSize());
			auto rectOffset = m_textureRect ? m_textureRect->position / texSize : glm::vec2{0.f};
			auto rectSize = m_textureRect ? m_textureRect->size / texSize : glm::vec2{1.f};
			rectOffset.y = 1.f - rectOffset.y - rectSize.y; // set origin to top left
			m_shader->setUniform("u_TexRectOffset", rectOffset);
			m_shader->setUniform("u_TexRectSize", rectSize);
		} else {
			m_shader->setUniform("u_UseTexture", false);
			m_shader->setUniform("u_TexRectOffset", glm::vec2{0.f});
			m_shader->setUniform("u_TexRectSize", glm::vec2{1.f});
		}

		for (auto const& [name, value] : m_floats) { m_shader->setUniform(name, value); }
		for (auto const& [name, value] : m_vec2s) { m_shader->setUniform(name, value); }
		for (auto const& [name, value] : m_vec3s) { m_shader->setUniform(name, value); }
		for (auto const& [name, value] : m_vec4s) { m_shader->setUniform(name, value); }
		for (auto const& [name, value] : m_ints) { m_shader->setUniform(name, value); }
		for (auto const& [name, value] : m_mat4s) { m_shader->setUniform(name, value); }
	}

	[[nodiscard]] auto getShader() const -> engine::Shader& { return *m_shader; }

  private:
	std::shared_ptr<Shader> m_shader{};
	Color m_color{};
	std::shared_ptr<Texture> m_texture{};
	std::optional<FloatRect> m_textureRect{};

	std::unordered_map<std::string, float> m_floats;
	std::unordered_map<std::string, glm::vec2> m_vec2s;
	std::unordered_map<std::string, glm::vec3> m_vec3s;
	std::unordered_map<std::string, glm::vec4> m_vec4s;
	std::unordered_map<std::string, int> m_ints;
	std::unordered_map<std::string, glm::mat4> m_mat4s;
};
} // namespace resurgo::engine