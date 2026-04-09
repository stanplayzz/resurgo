#include "resurgo/game/cursor.hpp"
#include "resurgo/engine/input.hpp"

namespace resurgo {
namespace {
constexpr auto cursorThickness_v = 0.05f;
constexpr auto cursorColor_v = Color::White;
constexpr auto cursorSpeed_v = 25.f;
constexpr auto blinkSpeed_v = 2.f;
constexpr auto cursorAlphaMin_v = 0.6f;
constexpr auto cursorAlphaMax_v = 1.0f;
} // namespace

Cursor::Cursor(clib::not_null<ChunkManager const*> chunkManager) : m_chunkManager(chunkManager) {
	m_cursor.transform.scale = {tileSize_v, tileSize_v, 1};
	auto shader = engine::Resources::instance().loadShader("Outline", ASSETS_DIR "/shaders/default.vert",
														   ASSETS_DIR "/shaders/outline.frag");
	m_cursor.material = engine::Material{shader};
	m_cursor.material.setFloat("u_OutlineThickness", cursorThickness_v);
	m_cursor.material.setVec4("u_OutlineColor", cursorColor_v.toVec4());
}

void Cursor::update(float deltaTime, engine::Camera const& camera) {
	if (auto pos = m_chunkManager->screenToTile(engine::Input::getMousePosition(), camera)) {
		m_target =
			glm::vec3{*pos} + glm::vec3{m_cursor.transform.scale.x * 0.5f, m_cursor.transform.scale.y * 0.5f, 0.1f};
	}
	m_cursor.transform.position += (m_target - m_cursor.transform.position) * cursorSpeed_v * deltaTime;

	m_elapsedTime += deltaTime;
	auto a = cursorAlphaMin_v +
			 (((std::sin(m_elapsedTime * blinkSpeed_v) * 0.5f) + 0.5f) * (cursorAlphaMax_v - cursorAlphaMin_v));
	m_cursor.material.setVec4("u_OutlineColor", glm::vec4{cursorColor_v.toVec3(), a});
}

void Cursor::draw(engine::Renderer& renderer) const { m_cursor.draw(renderer); }
} // namespace resurgo