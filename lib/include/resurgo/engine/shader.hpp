#pragma once
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <string>

namespace resurgo::engine {
class Shader {
  public:
	Shader(std::string const& vertPath, std::string const& fragPath);

	void use() const { glUseProgram(m_programID); }

	template <typename T>
	void setUniform(std::string const& name, T const& value) {
		int location = glGetUniformLocation(m_programID, name.c_str());
		if (location == -1) { return; }
		if constexpr (std::is_same_v<T, int>) {
			glUniform1i(location, value);
		} else if constexpr (std::is_same_v<T, float>) {
			glUniform1f(location, value);
		} else if constexpr (std::is_same_v<T, bool>) {
			glUniform1i(location, value);
		} else if constexpr (std::is_same_v<T, glm::vec2>) {
			glUniform2fv(location, 1, &value[0]);
		} else if constexpr (std::is_same_v<T, glm::vec3>) {
			glUniform3fv(location, 1, &value[0]);
		} else if constexpr (std::is_same_v<T, glm::vec4>) {
			glUniform4fv(location, 1, &value[0]);
		} else if constexpr (std::is_same_v<T, glm::mat3>) {
			glUniformMatrix3fv(location, 1, GL_FALSE, &value[0][0]);
		} else if constexpr (std::is_same_v<T, glm::mat4>) {
			glUniformMatrix4fv(location, 1, GL_FALSE, &value[0][0]);
		} else if constexpr (std::is_same_v<T, glm::mat<4, 4, int>>) {
			glUniformMatrix4fv(location, 1, GL_FALSE, &glm::mat4(value)[0][0]);
		} else {
			static_assert(!sizeof(T), "Unsupported uniform type");
		}
	}

  private:
	unsigned int m_programID{};
};
} // namespace resurgo::engine