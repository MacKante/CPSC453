#include <glad/glad.h>

#include <iostream>
#include <string>
#include <list>
#include <vector>
#include <limits>
#include <functional>

#include "Geometry.h"
#include "GLDebug.h"
#include "Log.h"
#include "ShaderProgram.h"
#include "Shader.h"
#include "Texture.h"
#include "Window.h"
#include "Panel.h"

#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

enum CURVE_TYPE {
	BEZIER,
	B_SPLINE
};

enum CURVE_TYPE curve_type = B_SPLINE;

class CurveEditorCallBack : public CallbackInterface {
public:
	CurveEditorCallBack() {}

	virtual void keyCallback(int key, int scancode, int action, int mods) override {
		Log::info("KeyCallback: key={}, action={}", key, action);
	}

	virtual void mouseButtonCallback(int button, int action, int mods) override {
		Log::info("MouseButtonCallback: button={}, action={}", button, action);
	}

	virtual void cursorPosCallback(double xpos, double ypos) override {
		Log::info("CursorPosCallback: xpos={}, ypos={}", xpos, ypos);
	}

	virtual void scrollCallback(double xoffset, double yoffset) override {
		Log::info("ScrollCallback: xoffset={}, yoffset={}", xoffset, yoffset);
	}

	virtual void windowSizeCallback(int width, int height) override {
		Log::info("WindowSizeCallback: width={}, height={}", width, height);
		CallbackInterface::windowSizeCallback(width, height); // Important, calls glViewport(0, 0, width, height);
	}
};

// Can swap the callback instead of maintaining a state machine
/*
class TurnTable3DViewerCallBack : public CallbackInterface {

public:
	TurnTable3DViewerCallBack() {}

	virtual void keyCallback(int key, int scancode, int action, int mods) {}
	virtual void mouseButtonCallback(int button, int action, int mods) {}
	virtual void cursorPosCallback(double xpos, double ypos) {}
	virtual void scrollCallback(double xoffset, double yoffset) {}
	virtual void windowSizeCallback(int width, int height) {

		// The CallbackInterface::windowSizeCallback will call glViewport for us
		CallbackInterface::windowSizeCallback(width, height);
	}
private:

};
*/

class CurveEditorPanelRenderer : public PanelRendererInterface {
public:
	CurveEditorPanelRenderer()
		: inputText(""), buttonClickCount(0), sliderValue(0.0f),
		dragValue(0.0f), inputValue(0.0f), checkboxValue(false),
		comboSelection(0)
	{
		// Initialize options for the combo box
		options[0] = "Option 1";
		options[1] = "Option 2";
		options[2] = "Option 3";

		// Initialize color (white by default)
		colorValue[0] = 1.0f; // R
		colorValue[1] = 1.0f; // G
		colorValue[2] = 1.0f; // B
	}

	virtual void render() override {
		// Color selector
		ImGui::ColorEdit3("Select Background Color", colorValue); // RGB color selector
		ImGui::Text("Selected Color: R: %.3f, G: %.3f, B: %.3f", colorValue[0], colorValue[1], colorValue[2]);

		// Text input
		ImGui::InputText("Input Text", inputText, IM_ARRAYSIZE(inputText));

		// Display the input text
		ImGui::Text("You entered: %s", inputText);

		// Button
		if (ImGui::Button("Click Me")) {
			buttonClickCount++;
		}
		ImGui::Text("Button clicked %d times", buttonClickCount);

		// Scrollable block
		ImGui::TextWrapped("Scrollable Block:");
		ImGui::BeginChild("ScrollableChild", ImVec2(0, 100), true); // Create a scrollable child
		for (int i = 0; i < 20; i++) {
			ImGui::Text("Item %d", i);
		}
		ImGui::EndChild();

		// Float slider
		ImGui::SliderFloat("Float Slider", &sliderValue, 0.0f, 100.0f, "Slider Value: %.3f");

		// Float drag
		ImGui::DragFloat("Float Drag", &dragValue, 0.1f, 0.0f, 100.0f, "Drag Value: %.3f");

		// Float input
		ImGui::InputFloat("Float Input", &inputValue, 0.1f, 1.0f, "Input Value: %.3f");

		// Checkbox
		ImGui::Checkbox("Enable Feature", &checkboxValue);
		ImGui::Text("Feature Enabled: %s", checkboxValue ? "Yes" : "No");

		// Combo box
		ImGui::Combo("Select an Option", &comboSelection, options, IM_ARRAYSIZE(options));
		ImGui::Text("Selected: %s", options[comboSelection]);

		// Displaying current values
		ImGui::Text("Slider Value: %.3f", sliderValue);
		ImGui::Text("Drag Value: %.3f", dragValue);
		ImGui::Text("Input Value: %.3f", inputValue);
	}

	glm::vec3 getColor() const {
		return glm::vec3(colorValue[0], colorValue[1], colorValue[2]);
	}

private:
	float colorValue[3];  // Array for RGB color values
	char inputText[256];  // Buffer for input text
	int buttonClickCount; // Count button clicks
	float sliderValue;    // Value for float slider
	float dragValue;      // Value for drag input
	float inputValue;     // Value for float input
	bool checkboxValue;   // Value for checkbox
	int comboSelection;   // Index of selected option in combo box
	const char* options[3]; // Options for the combo box
};

/*----------------------- de Casteljau Algorithm -----------------------*/

glm::vec3 deCasteljau(std::vector<glm::vec3>& controlPoints, float u) {
	// Copy of controlPoints to play with
	std::vector<glm::vec3> P = controlPoints;

	// Degree: n + 1
	int d = P.size();

	// Create smaller control points
	for (int i = 1; i < d; ++i) {
		for (int j = 0; j < (d - i); ++j) {
			P[j] = (1.0f - u) * P[j] + u * P[j + 1];
		}
	}

	// Return curve point 
	return P[0];
}

std::vector<glm::vec3> chaikinCurveSubdivision(std::vector<glm::vec3>& coarsePoints, int iterations = 8) {
	// No iterations or not enough points to subdivide
	int n = coarsePoints.size();
	if ((iterations == 0) || (n < 2)) {
		return coarsePoints;
	}

	std::vector<glm::vec3> C = coarsePoints;
	std::vector<glm::vec3> F;

	// Special mask at the beginning
	F.push_back(C[0]);
	F.push_back(0.5f*C[0] + 0.5f*C[1]);

	// Periodic mask for the interior points
	for (int i = 1; i < n - 2; ++i) {
		F.push_back(0.75f * C[i] + 0.25f * C[i + 1]);
		F.push_back(0.25f * C[i] + 0.75f * C[i + 1]);
	}

	// Special mask at the end
	F.push_back(0.5f * C[n - 2] + 0.5f * C[n - 1]);
	F.push_back(C[n - 1]);

	// Perform subdivision on fine points if there are more iterations
	return chaikinCurveSubdivision(F, iterations - 1);
}

int main() {
	Log::debug("Starting main");

	// WINDOW
	glfwInit();
	Window window(800, 800, "CPSC 453: Assignment 3");
	Panel panel(window.getGLFWwindow());

	//GLDebug::enable();

	// CALLBACKS
	auto curve_editor_callback = std::make_shared<CurveEditorCallBack>();
	//auto turn_table_3D_viewer_callback = std::make_shared<TurnTable3DViewerCallBack>();

	auto curve_editor_panel_renderer = std::make_shared<CurveEditorPanelRenderer>();

	//Set callback to window
	window.setCallbacks(curve_editor_callback);
	// Can swap the callback instead of maintaining a state machine
	//window.setCallbacks(turn_table_3D_viewer_callback);

	//Panel inputs
	panel.setPanelRenderer(curve_editor_panel_renderer);

	ShaderProgram shader_program_default(
		"shaders/test.vert",
		"shaders/test.frag"
	);

	std::vector<glm::vec3> cp_positions_vector = {
		{-0.8f, -0.4f, 0.0f},
		{-0.4f,  0.6f, 0.0f},
		{ 0.4f, -0.6f, 0.0f},
		{ 0.8f,  0.4f, 0.0f}
	};

	glm::vec3 cp_point_colour	= { 1.f,0.f,0.f };
	glm::vec3 cp_line_colour	= { 0.f,1.f,0.f };

	CPU_Geometry cp_point_cpu;
	cp_point_cpu.verts	= cp_positions_vector;
	cp_point_cpu.cols	= std::vector<glm::vec3>(cp_point_cpu.verts.size(), cp_point_colour);
	GPU_Geometry cp_point_gpu;
	cp_point_gpu.setVerts(cp_point_cpu.verts);
	cp_point_gpu.setCols(cp_point_cpu.cols);

	CPU_Geometry cp_line_cpu;
	cp_line_cpu.verts	= cp_positions_vector; // We are using GL_LINE_STRIP (change this if you want to use GL_LINES)
	cp_line_cpu.cols	= std::vector<glm::vec3>(cp_point_cpu.verts.size(), cp_line_colour);
	GPU_Geometry cp_line_gpu;
	cp_line_gpu.setVerts(cp_line_cpu.verts);
	cp_line_gpu.setCols(cp_line_cpu.cols);

	// curve geometry
	CPU_Geometry curve_cpu_geom;
	GPU_Geometry curve_gpu_geom;	

	while (!window.shouldClose()) {
		glfwPollEvents();
		glm::vec3 background_colour = curve_editor_panel_renderer->getColor();

		//------------------------------------------
		glEnable(GL_LINE_SMOOTH);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_FRAMEBUFFER_SRGB);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glClearColor(background_colour.r, background_colour.g, background_colour.b, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//------------------------------------------
		
		// Use the default shader (can use different ones for different objects)
		shader_program_default.use();

		//Render control points
		cp_point_gpu.bind();
		glPointSize(15.f);
		glDrawArrays(GL_POINTS, 0, cp_point_cpu.verts.size());

		//Render curve connecting control points
		cp_line_gpu.bind();
		//glLineWidth(10.f); //May do nothing (like it does on my computer): https://community.khronos.org/t/3-0-wide-lines-deprecated/55426
		glDrawArrays(GL_LINE_STRIP, 0, cp_line_cpu.verts.size());

		/* ----------------- Curve Rendering -----------------*/
			// Render curve points
			std::vector<glm::vec3> curve_points;

			// Generate points on the curve
			switch (curve_type) {
				case BEZIER:
					for (float u = 0.0f; u < 1.0f; u += 0.01f) {
						glm::vec3 curve_point = deCasteljau(cp_positions_vector, u);
						curve_points.push_back(curve_point);
					}
					break;
				case B_SPLINE:
					curve_points = chaikinCurveSubdivision(cp_positions_vector);
					break;
			}

			// Update CPU and GPU geometries
			curve_cpu_geom.verts = curve_points;
			curve_cpu_geom.cols = std::vector<glm::vec3>(curve_points.size(), {0.0f, 0.0f, 0.0f}); // black curve line
			curve_gpu_geom.setVerts(curve_cpu_geom.verts);
			curve_gpu_geom.setCols(curve_cpu_geom.cols);

			// Bind and draw the curve
			curve_gpu_geom.bind();
			glDrawArrays(GL_LINE_STRIP, 0, curve_cpu_geom.verts.size());
		/* ----------------- Curve Rendering End -----------------*/


		//------------------------------------------
		glDisable(GL_FRAMEBUFFER_SRGB); // disable sRGB for things like imgui
		panel.render();
		//------------------------------------------
		window.swapBuffers();
		//------------------------------------------
	}

	glfwTerminate();
	return 0;
}
