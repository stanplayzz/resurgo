#pragma once
#include "resurgo/engine/font.hpp"
#include "resurgo/engine/shader.hpp"
#include "resurgo/engine/texture.hpp"
#include <filesystem>
#include <memory>
#include <stdexcept>
#include <string>
#include <unordered_map>

namespace resurgo::engine {
class Resources {
  public:
	static Resources& get() {
		static Resources instance;
		return instance;
	}

	// Shaders
	auto loadShader(std::string const& name, std::string const& vertPath, std::string const& fragPath)
		-> std::shared_ptr<Shader> {
		if (auto it = m_shaders.find(name); it != m_shaders.end()) { return it->second; }

		auto shader = std::make_shared<Shader>(m_assetsDir / vertPath, m_assetsDir / fragPath);
		m_shaders[name] = shader;
		return shader;
	}

	auto getShader(std::string const& name) -> std::shared_ptr<Shader> {
		if (auto it = m_shaders.find(name); it != m_shaders.end()) { return it->second; }
		throw std::runtime_error{"Shader not found: " + name};
	}

	auto getDefaultShader() -> std::shared_ptr<Shader> {
		return loadShader("DEFAULT", "shaders/default.vert", "shaders/default.frag");
	}

	// Textures
	auto loadTexture(std::string const& path) -> std::shared_ptr<Texture> {
		if (auto it = m_textures.find(path); it != m_textures.end()) { return it->second; }

		auto texture = std::make_shared<Texture>(m_assetsDir / path);
		m_textures[path] = texture;
		return texture;
	}

	auto getTexture(std::string const& path) -> std::shared_ptr<Texture> {
		if (auto it = m_textures.find(path); it != m_textures.end()) { return it->second; }
		throw std::runtime_error{"Texture not found: " + path};
	}

	// Fonts
	auto loadFont(std::string const& path) -> std::shared_ptr<Font> {
		if (auto it = m_fonts.find(path); it != m_fonts.end()) { return it->second; }

		auto font = std::make_shared<Font>(m_assetsDir / path);
		m_fonts[path] = font;
		return font;
	}

	auto getFont(std::string const& path) -> std::shared_ptr<Font> {
		if (auto it = m_fonts.find(path); it != m_fonts.end()) { return it->second; }
		throw std::runtime_error{"Font not found: " + path};
	}

	void unloadShader(std::string const& name) { m_shaders.erase(name); }

	void unloadTexture(std::string const& path) { m_textures.erase(path); }

	void unloadFont(std::string const& path) { m_fonts.erase(path); }

	void unloadAll() {
		m_shaders.clear();
		m_textures.clear();
		m_fonts.clear();
	}

	void setAssetsDir(char const* argv0) {
		auto path = std::filesystem::canonical(argv0).parent_path();
		while (path.has_parent_path() && path != path.parent_path()) {
			if (std::filesystem::is_directory(path / "assets")) {
				m_assetsDir = path / "assets";
				return;
			}
			path = path.parent_path();
		}
		throw std::runtime_error{"Failed to find assets directory"};
	}

  private:
	Resources() = default;

	std::unordered_map<std::string, std::shared_ptr<Shader>> m_shaders;
	std::unordered_map<std::string, std::shared_ptr<Texture>> m_textures;
	std::unordered_map<std::string, std::shared_ptr<Font>> m_fonts;
	std::filesystem::path m_assetsDir{};
};

} // namespace resurgo::engine