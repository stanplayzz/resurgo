#include "resurgo/game/entrypoint.hpp"
#include "resurgo/engine/input.hpp"
#include "resurgo/game/gameplay.hpp"

namespace resurgo {
Entrypoint::Entrypoint()
	: m_title(*engine::Resources::get().getFont("fonts/audiowide.ttf")),
	  m_playButton(*engine::Resources::get().getFont("fonts/audiowide.ttf")) {
	m_camera.updateSize({1280, 720});
	m_camera.nearPlane = 0;

	m_title.setCharacterSize(64);
	m_title.setString("RESURGO");
	m_title.setPosition({50, 75});

	m_playButton.setCharacterSize(48);
	m_playButton.setString("PLAY");
	m_playButton.setPosition({50, 250});

	m_background.transform.scale = {m_camera.getSize(), 1};
	m_background.transform.position = {0, 0, -100};
	m_background.material.setTexture(engine::Resources::get().loadTexture("images/entrypoint_background.png"));

	m_planet.transform.scale = {m_camera.getSize().y, m_camera.getSize().y, 1};
	m_planet.transform.position = {0, -m_camera.getSize().y * 0.5f, 0};
	m_planet.material.setTexture(engine::Resources::get().loadTexture("images/entrypoint_planet.png"));
}

auto Entrypoint::update(float deltaTime) -> std::unique_ptr<State> {
	if (auto newSize = engine::Input::resized()) {
		m_camera.updateSize(*newSize);
		m_background.transform.scale = {*newSize, 1};
		m_planet.transform.scale = {newSize->y, newSize->y, 1};
		m_planet.transform.position = {0, -m_camera.getSize().y * 0.5f, 0};
	}
	if (engine::Input::isKeyPressed(GLFW_KEY_SPACE)) { return std::make_unique<Gameplay>(); }

	auto playBtnHitbox = m_playButton.getBounds();
	playBtnHitbox.position.x = 50.f;

	if (engine::Input::isMouseButtonReleased(GLFW_MOUSE_BUTTON_1)) {
		if (playBtnHitbox.contains(engine::Input::getMousePosition())) { return std::make_unique<Gameplay>(); }
	}

	if (playBtnHitbox.contains(engine::Input::getMousePosition())) {
		auto t = (70 - m_playButton.getPosition().x) * deltaTime * 15.f;
		m_playButton.setPosition(m_playButton.getPosition() + glm::vec2{t, 0});
	} else {
		auto t = (50 - m_playButton.getPosition().x) * deltaTime * 10.f;
		m_playButton.setPosition(m_playButton.getPosition() + glm::vec2{t, 0});
	}

	m_planet.transform.rotation.z -= glm::radians(5.f * deltaTime);

	return nullptr;
}

void Entrypoint::draw(engine::Renderer& renderer) const {
	renderer.setCamera(m_camera);
	m_background.draw(renderer);
	m_planet.draw(renderer);
	renderer.draw(m_title);
	renderer.draw(m_playButton);
}

} // namespace resurgo
