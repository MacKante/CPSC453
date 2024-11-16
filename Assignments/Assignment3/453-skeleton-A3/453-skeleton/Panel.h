#pragma once
#include <glad/glad.h>

#include <memory>
#include <imgui.h>
#include <GLFW/glfw3.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include "Window.h"

// Class that specifies render call for Panel
class PanelRendererInterface {
public:
	virtual void render() {}
};

// Functor to safely destroy ImGui contexts
struct PanelDeleter {
	void operator()(ImGuiContext* context) const {
		ImGui::DestroyContext(context);
	}
};

// The Panel class manages the lifecycle of ImGui with GLFW integration
class Panel {
public:
	// multi_view_port_enabled allows the panel to be moved out of the window, though it seems to be a bit buggy on some machines
	Panel(GLFWwindow* share);
	Panel(std::shared_ptr<PanelRendererInterface> panelRender, GLFWwindow* share);
	~Panel();

	void setPanelRenderer(std::shared_ptr<PanelRendererInterface> panelRenderer);

	void render();        // Render content here

private:
	std::unique_ptr<ImGuiContext, PanelDeleter> imguiContext;  // Manage ImGui context
	std::shared_ptr<PanelRendererInterface> panelRenderer;      // optional shared owning ptr (user provided)

};
