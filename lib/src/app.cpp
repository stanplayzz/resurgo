#include "resurgo/app.hpp"
#include "resurgo/build_version.hpp"
#include "resurgo/engine/input.hpp"
#include <format>
#include <stdexcept>

namespace resurgo {
namespace {
constexpr auto cameraSpeed_v = 100.f;
constexpr auto windowWidth_v = 1280;
constexpr auto windowHeight_v = 720;
} // namespace

App::App() {
	if (!glfwInit()) { throw std::runtime_error{"Failed to initialize GLFW"}; }

	m_window = glfwCreateWindow(windowWidth_v, windowHeight_v, std::format("Resurgo {}", buildVersionStr_v).c_str(),
								nullptr, nullptr);
	glfwMakeContextCurrent(m_window);

	// NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
	if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
		throw std::runtime_error{"Failed to initialize GLAD"};
	}

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	engine::Input::init(m_window);
	m_camera.updateSize({windowWidth_v, windowHeight_v});
}

App::~App() {
	glfwDestroyWindow(m_window);
	glfwTerminate();
}

void App::run() {
	auto lastTime = static_cast<float>(glfwGetTime());

	// camera transform for dimetric view
	m_camera.transform.rotation = {glm::radians(90.f) - std::atan(std::sin(glm::radians(30.f))), 0.f,
								   glm::radians(45.f)};

	while (!glfwWindowShouldClose(m_window)) {
		auto currentTime = static_cast<float>(glfwGetTime());
		auto deltaTime = currentTime - lastTime;
		lastTime = currentTime;

		engine::Input::update();
		if (auto size = engine::Input::resized()) { m_camera.updateSize(*size); }
		transformCamera(deltaTime);

		m_chunkManager.update({0, 0});

		m_renderer.clear(Color::Black);
		m_renderer.begin(m_camera);

		m_chunkManager.draw(m_renderer);

		m_renderer.end();
		glfwSwapBuffers(m_window);
	}
}

void App::transformCamera(float deltaTime) {
	if (engine::Input::isKeyPressed(GLFW_KEY_W)) { m_camera.transform.position.y += cameraSpeed_v * deltaTime; }
	if (engine::Input::isKeyPressed(GLFW_KEY_S)) { m_camera.transform.position.y -= cameraSpeed_v * deltaTime; }
	if (engine::Input::isKeyPressed(GLFW_KEY_A)) { m_camera.transform.position.x -= cameraSpeed_v * deltaTime; }
	if (engine::Input::isKeyPressed(GLFW_KEY_D)) { m_camera.transform.position.x += cameraSpeed_v * deltaTime; }
}
} // namespace resurgo