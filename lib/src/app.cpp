#include "resurgo/app.hpp"
#include "resurgo/build_version.hpp"
#include "resurgo/engine/input.hpp"
#include "resurgo/engine/text.hpp"
#include "resurgo/game/entrypoint.hpp"
#include <format>
#include <stdexcept>

namespace resurgo {

App::App() {
	if (!glfwInit()) { throw std::runtime_error{"Failed to initialize GLFW"}; }

	m_window = glfwCreateWindow(m_windowSize.x, m_windowSize.y, std::format("Resurgo {}", buildVersionStr_v).c_str(),
								nullptr, nullptr);
	glfwMakeContextCurrent(m_window);

	// NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
	if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
		throw std::runtime_error{"Failed to initialize GLAD"};
	}

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	engine::Input::init(m_window);
}

App::~App() {
	glfwDestroyWindow(m_window);
	glfwTerminate();
}

void App::run() {
	preloadResources();

	auto lastTime = static_cast<float>(glfwGetTime());

	m_stateManager.switchState(std::make_unique<Entrypoint>());

	while (!glfwWindowShouldClose(m_window)) {
		auto currentTime = static_cast<float>(glfwGetTime());
		auto deltaTime = currentTime - lastTime;
		lastTime = currentTime;

		engine::Input::update();
		if (auto size = engine::Input::resized()) { m_windowSize = *size; }
		m_stateManager.update(deltaTime);

		m_renderer.clear(Color::Black);
		m_renderer.begin();

		m_stateManager.draw(m_renderer);

		m_renderer.end();
		glfwSwapBuffers(m_window);
	}
}

// NOLINTNEXTLINE(readability-convert-member-functions-to-static)
void App::preloadResources() {
	engine::Resources::get().loadFont("fonts/roboto.ttf");
	engine::Resources::get().loadFont("fonts/audiowide.ttf");
}
} // namespace resurgo