#pragma once
#include "resurgo/engine/shader.hpp"
#include "resurgo/engine/texture.hpp"
#include <memory>
#include <stdexcept>
#include <string>
#include <unordered_map>

namespace resurgo::engine {

class Resources {
  public:
	static Resources& instance() {
		static Resources instance;
		return instance;
	}

	// Shaders
	auto loadShader(std::string const& name, std::string const& vertPath, std::string const& fragPath)
		-> std::shared_ptr<Shader> {
		if (auto it = m_shaders.find(name); it != m_shaders.end()) { return it->second; }

		auto shader = std::make_shared<Shader>(vertPath, fragPath);
		m_shaders[name] = shader;
		return shader;
	}

	auto getShader(std::string const& name) -> std::shared_ptr<Shader> {
		if (auto it = m_shaders.find(name); it != m_shaders.end()) { return it->second; }
		throw std::runtime_error{"Shader not found: " + name};
	}

	auto getDefaultShader() -> std::shared_ptr<Shader> {
		return loadShader("DEFAULT", ASSETS_DIR "/shaders/default.vert", ASSETS_DIR "/shaders/default.frag");
	}

	// Textures
	auto loadTexture(std::string const& path) -> std::shared_ptr<Texture> {
		if (auto it = m_textures.find(path); it != m_textures.end()) { return it->second; }

		auto texture = std::make_shared<Texture>(path);
		m_textures[path] = texture;
		return texture;
	}

	auto getTexture(std::string const& path) -> std::shared_ptr<Texture> {
		if (auto it = m_textures.find(path); it != m_textures.end()) { return it->second; }
		throw std::runtime_error{"Texture not found: " + path};
	}

	void unloadShader(std::string const& name) { m_shaders.erase(name); }

	void unloadTexture(std::string const& path) { m_textures.erase(path); }

	void unloadAll() {
		m_shaders.clear();
		m_textures.clear();
	}

  private:
	Resources() = default;

	std::unordered_map<std::string, std::shared_ptr<Shader>> m_shaders;
	std::unordered_map<std::string, std::shared_ptr<Texture>> m_textures;
};

} // namespace resurgo::engine