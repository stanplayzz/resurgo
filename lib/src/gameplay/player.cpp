#include "resurgo/gameplay/player.hpp"
#include "resurgo/gameplay/iso_transform.hpp"
#include "resurgo/resources.hpp"
#include <cmath>

namespace resurgo {
namespace {
constexpr auto m_moveSpeed_v = 200.f;
constexpr auto getElevationAt(ChunkManager const& chunks, sf::Vector2f screenPos) -> int {
	auto bestZ = 0;

	for (auto const& [coord, chunk] : chunks.allChunks()) {
		for (auto const& tile : chunk.tiles()) {
			auto pos = IsoTransform::tileToScreen({tile.position.x, tile.position.y});

			auto cx = pos.x + (tileSize_v * 0.5f);
			auto cy = pos.y + (tileSize_v * 0.25f);

			auto hw = tileSize_v * 0.5f;
			auto hh = tileSize_v * 0.25f;

			auto inside = (std::abs(screenPos.x - cx) / hw) + (std::abs(screenPos.y - cy) / hh) <= 1.0f;

			if (inside && tile.position.z > bestZ) { bestZ = tile.position.z; }
		}
	}
	return bestZ;
};
} // namespace

Player::Player() : m_player{*Resources::instance().load<sf::Texture>("images/player.png")} {
	m_player.setScale({2, 2});

	m_hitbox.size = m_player.getGlobalBounds().size;
	m_hitbox.size.x *= 0.6f;
	m_hitbox.position = m_player.getGlobalBounds().getCenter() - m_hitbox.size * 0.5f;
}

void Player::update(sf::Time deltaTime, ChunkManager const& chunks) {
	// normalize diagonal movement
	auto dir = m_moveDirection;
	auto len = std::sqrt((dir.x * dir.x) + (dir.y * dir.y));
	dir = len > 0.f ? dir / len : sf::Vector2f{0, 0};

	m_player.setPosition(m_player.getPosition() + dir * deltaTime.asSeconds() * m_moveSpeed_v);

	m_hitbox.position = m_player.getGlobalBounds().getCenter() - m_hitbox.size * 0.5f;

	// correct y position relative to z
	auto playerPos = sf::Vector2f{m_hitbox.getCenter().x, m_hitbox.position.y + m_hitbox.size.y};
	m_elevation = 0;
	constexpr auto samples = 5;
	for (auto i = 0; i < samples; i++) {
		auto t = static_cast<float>(i) / (samples - 1);
		auto x = (m_hitbox.position.x) + (t * m_hitbox.size.x);
		m_elevation = std::max(m_elevation, getElevationAt(chunks, {x, playerPos.y}));
	}

	m_hitbox.position.y += static_cast<float>(-m_elevation * tileSize_v) * 0.25f;
}

void Player::draw(sf::RenderTarget& target, Settings const& settings) const {

	sf::RenderStates playerStates{};
	playerStates.transform.translate({0, static_cast<float>(-m_elevation * tileSize_v) * 0.25f});
	target.draw(m_player, playerStates);

	if (settings.drawHitboxes) {
		static sf::RectangleShape hitbox{};
		hitbox.setPosition(m_hitbox.position);
		hitbox.setSize(m_hitbox.size);
		hitbox.setFillColor(sf::Color::Transparent);
		hitbox.setOutlineThickness(2);
		target.draw(hitbox);
	}
}

void Player::handleInput(sf::Event const& event) {
	if (auto const* key = event.getIf<sf::Event::KeyPressed>()) {
		if (key->scancode == sf::Keyboard::Scancode::W) { m_moveDirection.y = -1; }
		if (key->scancode == sf::Keyboard::Scancode::S) { m_moveDirection.y = 1; }
		if (key->scancode == sf::Keyboard::Scancode::A) { m_moveDirection.x = -1; }
		if (key->scancode == sf::Keyboard::Scancode::D) { m_moveDirection.x = 1; }
	}
	if (auto const* key = event.getIf<sf::Event::KeyReleased>()) {
		if (key->scancode == sf::Keyboard::Scancode::W) { m_moveDirection.y = 0; }
		if (key->scancode == sf::Keyboard::Scancode::S) { m_moveDirection.y = 0; }
		if (key->scancode == sf::Keyboard::Scancode::A) { m_moveDirection.x = 0; }
		if (key->scancode == sf::Keyboard::Scancode::D) { m_moveDirection.x = 0; }
	}
}
} // namespace resurgo