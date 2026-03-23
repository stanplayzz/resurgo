#pragma once
#include "resurgo/engine/material.hpp"
#include "resurgo/engine/renderer.hpp"
#include "resurgo/engine/transform.hpp"
#include <clib/base_types.hpp>
#include <utility>

namespace resurgo::engine {
class GameObject : clib::Polymorphic {
  public:
	GameObject(Geometry geometry, std::shared_ptr<Shader> shader)
		: material(std::move(shader)), m_geometry(std::move(geometry)) {}

	void draw(Renderer& renderer) const { renderer.draw(*this); }

	Transform transform{};
	Material material{};

	[[nodiscard]] auto getGeometry() const -> Geometry const& { return m_geometry; }

  private:
	Geometry m_geometry;
};
} // namespace resurgo::engine