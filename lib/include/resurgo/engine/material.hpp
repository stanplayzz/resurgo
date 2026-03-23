#pragma once
#include "resurgo/engine/resources.hpp"
#include "resurgo/utils/color.hpp"
#include <memory>
#include <utility>

namespace resurgo::engine {
class Material {
  public:
	explicit Material(std::shared_ptr<Shader> shader = Resources::instance().getDefaultShader())
		: m_shader(std::move(shader)) {}

	void setColor(Color const& color) { m_color = color; }
	void setTexture(std::shared_ptr<Texture> texture) { m_texture = std::move(texture); }

	void setFloat(std::string const& name, float value) { m_floats[name] = value; }
	void setVec3(std::string const& name, glm::vec3 value) { m_vec3s[name] = value; }

	void bind() const {
		m_shader->use();
		m_shader->setUniform("u_Color", m_color.toVec4());

		if (m_texture) {
			m_shader->setUniform("u_UseTexture", true);
			m_texture->bind(0);
			m_shader->setUniform("u_Texture", 0);
		} else {
			m_shader->setUniform("u_UseTexture", false);
		}

		for (auto const& [name, value] : m_floats) { m_shader->setUniform(name, value); }
		for (auto const& [name, value] : m_vec3s) { m_shader->setUniform(name, value); }
	}

	[[nodiscard]] auto getShader() const -> engine::Shader& { return *m_shader; }

  private:
	std::shared_ptr<Shader> m_shader{};
	Color m_color{};
	std::shared_ptr<Texture> m_texture{};
	std::unordered_map<std::string, float> m_floats{};
	std::unordered_map<std::string, glm::vec3> m_vec3s{};
};
} // namespace resurgo::engine