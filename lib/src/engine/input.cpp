#include "resurgo/engine/input.hpp"
#include <GLFW/glfw3.h>

namespace resurgo::engine {
GLFWwindow* Input::s_window = nullptr;
std::unordered_set<int> Input::s_keysPressed;
std::unordered_set<int> Input::s_mouseButtonsPressed;
glm::vec2 Input::s_mousePosition{};
glm::vec2 Input::s_lastMousePosition{};
glm::vec2 Input::s_scrollDelta{};
glm::vec2 Input::s_windowSize{};
glm::vec2 Input::s_lastWindowSize{};

void Input::init(GLFWwindow* window) {
	s_window = window;

	int width{};
	int height{};
	glfwGetFramebufferSize(window, &width, &height);
	s_windowSize = glm::vec2{width, height};
	s_lastWindowSize = s_windowSize;

	glfwSetKeyCallback(window, keyCallback);
	glfwSetMouseButtonCallback(window, mouseButtonCallback);
	glfwSetCursorPosCallback(window, cursorPositionCallback);
	glfwSetScrollCallback(window, scrollCallback);
	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
}

void Input::update() {
	s_lastMousePosition = s_mousePosition;
	s_scrollDelta = glm::vec2{0.0f, 0.0f};
	s_lastWindowSize = s_windowSize;

	glfwPollEvents();
}

auto Input::isKeyPressed(int key) -> bool { return s_keysPressed.contains(key); }

auto Input::isMouseButtonPressed(int button) -> bool { return s_mouseButtonsPressed.contains(button); }
auto Input::getMousePosition() -> glm::vec2 { return s_mousePosition; }
auto Input::getMouseDelta() -> glm::vec2 { return s_mousePosition - s_lastMousePosition; }
auto Input::getScrollDelta() -> glm::vec2 { return s_scrollDelta; }

auto Input::resized() -> std::optional<glm::vec2> {
	if (s_lastWindowSize != s_windowSize) { return s_windowSize; }
	return {};
}

void Input::keyCallback(GLFWwindow* /*window*/, int key, int /*scancode*/, int action, int /*mods*/) {
	if (action == GLFW_PRESS) {
		s_keysPressed.insert(key);
	} else if (action == GLFW_RELEASE) {
		s_keysPressed.erase(key);
	}
}

void Input::mouseButtonCallback(GLFWwindow* /*window*/, int button, int action, int /*mods*/) {
	if (action == GLFW_PRESS) {
		s_mouseButtonsPressed.insert(button);
	} else if (action == GLFW_RELEASE) {
		s_mouseButtonsPressed.erase(button);
	}
}

void Input::cursorPositionCallback(GLFWwindow* /*window*/, double xpos, double ypos) {
	s_mousePosition = glm::vec2{static_cast<float>(xpos), static_cast<float>(ypos)};
}

void Input::scrollCallback(GLFWwindow* /*window*/, double xoffset, double yoffset) {
	s_scrollDelta = glm::vec2{static_cast<float>(xoffset), static_cast<float>(yoffset)};
}

void Input::framebufferSizeCallback(GLFWwindow* /*window*/, int width, int height) {
	glViewport(0, 0, width, height);
	s_windowSize = glm::vec2{width, height};
}

} // namespace resurgo::engine