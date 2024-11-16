#include "Window.h"

#include "Log.h"
#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

#include <iostream>


// ---------------------------
// static function definitions
// ---------------------------

void Window::keyMetaCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	CallbackInterface* callbacks = static_cast<CallbackInterface*>(glfwGetWindowUserPointer(window));

	bool forward_to_user_callback = true;
	// Forward the key event to ImGui
	if (ImGui::GetCurrentContext()) {
		ImGui_ImplGlfw_KeyCallback(window, key, scancode, action, mods);
		forward_to_user_callback = !ImGui::GetIO().WantCaptureKeyboard;
	}

	// If ImGui doesn't want to capture the keyboard, call the user-defined callback
	if (forward_to_user_callback && callbacks) {
		callbacks->keyCallback(key, scancode, action, mods);
	}
}


void Window::mouseButtonMetaCallback(GLFWwindow* window, int button, int action, int mods) {
	CallbackInterface* callbacks = static_cast<CallbackInterface*>(glfwGetWindowUserPointer(window));

	bool forward_to_user_callback = true;
	// Forward the event to ImGui
	if (ImGui::GetCurrentContext()) {
		ImGui_ImplGlfw_MouseButtonCallback(window, button, action, mods);
		forward_to_user_callback = !ImGui::GetIO().WantCaptureMouse;
	}

	// If ImGui doesn't want to capture the mouse, call the user-defined callback
	if (forward_to_user_callback && callbacks) {
		callbacks->mouseButtonCallback(button, action, mods);
	}
}


void Window::cursorPosMetaCallback(GLFWwindow* window, double xpos, double ypos) {
	CallbackInterface* callbacks = static_cast<CallbackInterface*>(glfwGetWindowUserPointer(window));

	bool forward_to_user_callback = true;
	// Forward the event to ImGui
	if (ImGui::GetCurrentContext()) {
		ImGui_ImplGlfw_CursorPosCallback(window, xpos, ypos);
		forward_to_user_callback = !ImGui::GetIO().WantCaptureMouse;
	}

	// Call user-defined cursor position callback only if ImGui isn't capturing the mouse
	if (forward_to_user_callback && callbacks) {
		callbacks->cursorPosCallback(xpos, ypos);
	}
}


void Window::scrollMetaCallback(GLFWwindow* window, double xoffset, double yoffset) {
	CallbackInterface* callbacks = static_cast<CallbackInterface*>(glfwGetWindowUserPointer(window));

	bool forward_to_user_callback = true;
	// Forward the scroll event to ImGui
	if (ImGui::GetCurrentContext()) {
		ImGui_ImplGlfw_ScrollCallback(window, xoffset, yoffset);
		forward_to_user_callback = !ImGui::GetIO().WantCaptureMouse;
	}

	// If ImGui isn't capturing scroll input, call the user-defined scroll callback
	if (forward_to_user_callback && callbacks) {
		callbacks->scrollCallback(xoffset, yoffset);
	}
}


void Window::windowSizeMetaCallback(GLFWwindow* window, int width, int height) {
	CallbackInterface* callbacks = static_cast<CallbackInterface*>(glfwGetWindowUserPointer(window));

	// Call the user-defined callback for window resizing
	if (callbacks) {
		callbacks->windowSizeCallback(width, height);
	}
}

// ----------------------
// non-static definitions
// ----------------------

Window::Window(
	std::shared_ptr<CallbackInterface> callbacks, int width, int height,
	const char* title, GLFWmonitor* monitor, GLFWwindow* share
)
	: window(nullptr)
	, callbacks(callbacks)
{
	// specify OpenGL version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // needed for mac?
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

	// create window
	window = std::unique_ptr<GLFWwindow, WindowDeleter>(glfwCreateWindow(width, height, title, monitor, share));
	if (window == nullptr) {
		Log::error("WINDOW failed to create GLFW window");
		throw std::runtime_error("Failed to create GLFW window.");
	}
	glfwMakeContextCurrent(window.get());

	// initialize OpenGL extensions for the current context (this window)
	if (!gladLoadGL()) {
		throw std::runtime_error("Failed to initialize GLAD");
	}

	glfwSetWindowSizeCallback(window.get(), defaultWindowSizeCallback);

	if (callbacks != nullptr) {
		connectCallbacks();
	}

}


Window::Window(int width, int height, const char* title, GLFWmonitor* monitor, GLFWwindow* share)
	: Window(nullptr, width, height, title, monitor, share)
{}


void Window::connectCallbacks() {
	// set userdata of window to point to the object that carries out the callbacks
	glfwSetWindowUserPointer(window.get(), callbacks.get());

	// bind meta callbacks to actual callbacks
	glfwSetKeyCallback(window.get(), keyMetaCallback);
	glfwSetMouseButtonCallback(window.get(), mouseButtonMetaCallback);
	glfwSetCursorPosCallback(window.get(), cursorPosMetaCallback);
	glfwSetScrollCallback(window.get(), scrollMetaCallback);
	glfwSetWindowSizeCallback(window.get(), windowSizeMetaCallback);
}


void Window::setCallbacks(std::shared_ptr<CallbackInterface> callbacks_) {
	callbacks = callbacks_;
	connectCallbacks();
}


glm::ivec2 Window::getPos() const {
	int x, y;
	glfwGetWindowPos(window.get(), &x, &y);
	return glm::ivec2(x, y);
}


glm::ivec2 Window::getSize() const {
	int w, h;
	glfwGetWindowSize(window.get(), &w, &h);
	return glm::ivec2(w, h);
}
