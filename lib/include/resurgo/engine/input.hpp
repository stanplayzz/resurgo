#pragma once
#include <GLFW/glfw3.h>
#include <clib/not_null.hpp>
#include <glm/vec2.hpp>
#include <optional>
#include <unordered_set>

namespace resurgo::engine {
class Input {
  public:
	static void init(GLFWwindow* window);
	static void update();

	[[nodiscard]] static auto isKeyPressed(int key) -> bool;

	[[nodiscard]] static auto isMouseButtonPressed(int button) -> bool;
	[[nodiscard]] static auto isMouseButtonClicked(int button) -> bool;
	[[nodiscard]] static auto isMouseButtonReleased(int button) -> bool;
	[[nodiscard]] static auto getMousePosition() -> glm::vec2;
	[[nodiscard]] static auto getMouseDelta() -> glm::vec2;
	[[nodiscard]] static auto getScrollDelta() -> glm::vec2;
	[[nodiscard]] static auto resized() -> std::optional<glm::vec2>;

  private:
	static GLFWwindow* s_window;

	static std::unordered_set<int> s_keysPressed;
	static std::unordered_set<int> s_mouseButtonsPressed;
	static std::unordered_set<int> s_prevMouseButtons;

	static glm::vec2 s_mousePosition;
	static glm::vec2 s_lastMousePosition;
	static glm::vec2 s_scrollDelta;

	static glm::vec2 s_windowSize;
	static glm::vec2 s_lastWindowSize;

	// GLFW callbacks
	static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
	static void cursorPositionCallback(GLFWwindow* window, double xpos, double ypos);
	static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
	static void framebufferSizeCallback(GLFWwindow* window, int width, int height);
};

} // namespace resurgo::engine