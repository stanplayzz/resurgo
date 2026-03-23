#include "resurgo/engine/shader.hpp"
#include <fstream>
#include <stdexcept>
#include <string>

namespace resurgo::engine {
namespace {
constexpr auto readFile = [](std::string const& path) {
	std::ifstream file{path};
	if (!file) { throw std::runtime_error{"Failed to open shader: " + path}; }
	return std::string{std::istreambuf_iterator<char>{file}, std::istreambuf_iterator<char>{}};
};

constexpr auto checkCompileErrors(unsigned int shader, std::string const& type) {
	auto succes = int{};
	auto infoLog = std::array<char, 1024>{};
	glGetShaderiv(shader, GL_COMPILE_STATUS, &succes);
	if (!succes) {
		glGetShaderInfoLog(shader, infoLog.size(), nullptr, infoLog.data());
		throw std::runtime_error{std::format("Shader compilation error ({}): {}", type, infoLog.data())};
	}
}

constexpr auto checkLinkingErrors(unsigned int program) {
	auto succes = int{};
	auto infoLog = std::array<char, 1024>{};
	glGetProgramiv(program, GL_LINK_STATUS, &succes);
	if (!succes) {
		glGetProgramInfoLog(program, infoLog.size(), nullptr, infoLog.data());
		throw std::runtime_error{std::format("Shader linking error: {}", infoLog.data())};
	}
}
} // namespace

Shader::Shader(std::string const& vertPath, std::string const& fragPath) {
	auto vertCode = readFile(vertPath);
	auto fragCode = readFile(fragPath);
	auto const* vertStr = vertCode.c_str();
	auto const* fragStr = fragCode.c_str();

	auto vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vertStr, nullptr);
	glCompileShader(vertex);
	checkCompileErrors(vertex, "VERTEX");

	auto fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fragStr, nullptr);
	glCompileShader(fragment);
	checkCompileErrors(fragment, "FRAGMENT");

	m_programID = glCreateProgram(); // NOLINT(cppcoreguidelines-prefer-member-initializer)
	glAttachShader(m_programID, vertex);
	glAttachShader(m_programID, fragment);
	glLinkProgram(m_programID);
	checkLinkingErrors(m_programID);

	glDeleteShader(vertex);
	glDeleteShader(fragment);
}
} // namespace resurgo::engine