#include "resurgo/game/player.hpp"
#include "resurgo/engine/input.hpp"
#include "resurgo/game/dimetric.hpp"
#include <glm/gtc/quaternion.hpp>

namespace resurgo {
namespace {
constexpr auto playerTextureSize_v = glm::ivec2{31, 56};
constexpr auto playerScale_v = 4;
constexpr auto playerSpeed_v = 200.f;
constexpr auto animSpeed_v = 0.3f; // seconds per frame
} // namespace
Player::Player() {
	m_player.transform.scale = {playerTextureSize_v * playerScale_v, 1};
	m_player.transform.rotation = glm::eulerAngles(glm::quat(dimetricView_v));
	m_player.material.setColor(Color::Red);

	auto texture = engine::Resources::instance().loadTexture(ASSETS_DIR "/images/player.png");
	m_player.material.setTexture(texture);
	m_player.material.setTextureRect({.size = playerTextureSize_v});
}

void Player::update(float deltaTime, ChunkManager const& chunks) {
	handleInput();

	m_player.transform.position += glm::vec3{m_moveDirection * playerSpeed_v * deltaTime, 0};

	engine::FloatRect hitbox{};
	hitbox.size = m_player.transform.scale * 0.8f;
	hitbox.position = getPosition();

	auto centerChunk = glm::ivec2{std::floor(hitbox.position.x / (chunkSize_v * tileSize_v)),
								  std::floor(hitbox.position.y / (chunkSize_v * tileSize_v))};
	auto mod = [](int a, int b) {
		return ((a % b) + b) % b;
	};
	auto tileX = mod(static_cast<int>(std::floor(hitbox.position.x / tileSize_v)), chunkSize_v);
	auto tileY = mod(static_cast<int>(std::floor(hitbox.position.y / tileSize_v)), chunkSize_v);
	auto baseZ = chunks.getChunkAt(centerChunk)
					 ->tileAt(static_cast<std::size_t>(tileX + (tileY * chunkSize_v)))
					 .transform.position.z;
	m_player.transform.position.z = baseZ + (hitbox.size.y * 0.5f);

	animate(deltaTime);
}

void Player::draw(engine::Renderer& renderer) const { m_player.draw(renderer); }

void Player::handleInput() {
	m_moveDirection = {};
	if (engine::Input::isKeyPressed(GLFW_KEY_W)) { m_moveDirection.y = 1; }
	if (engine::Input::isKeyPressed(GLFW_KEY_S)) { m_moveDirection.y = -1; }
	if (engine::Input::isKeyPressed(GLFW_KEY_A)) {
		m_moveDirection.x = -1;
		m_flipped = true;
	}
	if (engine::Input::isKeyPressed(GLFW_KEY_D)) {
		m_moveDirection.x = 1;
		m_flipped = false;
	}
	if (glm::length(m_moveDirection) > 0.f) { m_moveDirection = glm::normalize(m_moveDirection); }
}

void Player::animate(float deltaTime) {
	m_animTime += deltaTime;
	if (m_animTime > animSpeed_v) {
		m_animTime -= animSpeed_v;

		auto moving = m_moveDirection.x != 0 || m_moveDirection.y != 0;
		auto id = 0;
		if (moving) {
			id = ((m_currentFrame + 1) % 5);
			if (id == 0) { id = 1; }
		}
		m_currentFrame = id;

		setSprite(id, m_flipped);
	}
}

void Player::setSprite(int id, bool flipped) {
	m_player.material.setTextureRect({
		.size = {playerTextureSize_v.x * (flipped ? -1 : 1), playerTextureSize_v.y},
		.position = {flipped ? playerTextureSize_v.x : 0.f, id * playerTextureSize_v.y},
	});
}
} // namespace resurgo