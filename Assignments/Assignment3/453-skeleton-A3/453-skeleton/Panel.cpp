#include "Panel.h"
#include "Log.h"
#include <imgui_internal.h>

Panel::Panel(std::shared_ptr<PanelRendererInterface> panelRender, GLFWwindow* share)
	: imguiContext(ImGui::CreateContext()), panelRenderer(panelRender)  // Create ImGui context
{
	if (!imguiContext) {
		Log::error("Failed to create ImGui context");
		throw std::runtime_error("Failed to create ImGui context.");
	}

	ImGui::SetCurrentContext(imguiContext.get());

	// Initialize GLFW and OpenGL3 bindings
	if (!ImGui_ImplGlfw_InitForOpenGL(share, true)) {
		Log::error("Failed to initialize ImGui GLFW backend");
		throw std::runtime_error("Failed to initialize ImGui GLFW backend.");
	}
	if (!ImGui_ImplOpenGL3_Init("#version 330")) {
		Log::error("Failed to initialize ImGui OpenGL3 backend");
		throw std::runtime_error("Failed to initialize ImGui OpenGL3 backend.");
	}

	// Set ImGui style
	ImGui::StyleColorsDark();

}

Panel::Panel(GLFWwindow* share) : Panel(nullptr, share) {}

Panel::~Panel() {
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
}

void Panel::setPanelRenderer(std::shared_ptr<PanelRendererInterface> panelRenderer_) {
	panelRenderer = panelRenderer_;
}

void Panel::render() {
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	if (panelRenderer) {  // Check if the panel renderer is set
		panelRenderer->render();
	}

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
