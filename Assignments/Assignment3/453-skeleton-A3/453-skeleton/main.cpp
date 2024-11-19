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

/*-------------------------------- Macros and Enums --------------------------------*/
#define WINDOW_HEIGHT 800
#define WINDOW_WIDTH 800

#define POINT_PROXIMITY_THRESHOLD 0.1f

enum CURVE_TYPE {
	BEZIER,
	B_SPLINE
};

enum PROGRAM_MODE {
	CURVE_EDITOR,
	ORBIT_VIEWER
};

enum POINT_MODE {
	SELECT_MODE,
	INSERT_MODE,
	DELETE_MODE
};

/*------------------------------------ Structs ------------------------------------*/
struct CurveEditorCallbackInput
{
	glm::vec3 cursorPos = glm::vec3(0.0f, 0.0f, 0.0f);
	bool mousePress = false;
	enum CURVE_TYPE curveType = BEZIER;
};

struct CurveEditorPanelInput
{
	enum CURVE_TYPE curveType = BEZIER;
	enum PROGRAM_MODE programMode = CURVE_EDITOR;
	enum POINT_MODE pointMode = SELECT_MODE;
};
/*--------------------------------- Global Static ---------------------------------*/
static bool letGo;
static bool resetPoints = false;


class CurveEditorCallBack : public CallbackInterface {
public:
	CurveEditorCallBack() {}

	virtual void keyCallback(int key, int scancode, int action, int mods) override {
		Log::info("KeyCallback: key={}, action={}", key, action);

		if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
			if (input.curveType == BEZIER) {
				input.curveType = B_SPLINE;
			}
			else {
				input.curveType = BEZIER;
			}
		}
	}

	virtual void mouseButtonCallback(int button, int action, int mods) override {
		

		if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
			input.mousePress = true;
		}
		else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
			input.mousePress = false;
			letGo = true;
		}

		Log::info("MouseButtonCallback: button={}, action={}", button, action);
	}

	virtual void cursorPosCallback(double xpos, double ypos) override {
		// Calculate cursor position in -1 to 1 range
		glm::vec2 startingVec(xpos, ypos);
		glm::vec2 shiftedVec = startingVec + glm::vec2(0.5f, 0.5f);
		glm::vec2 scaledToZeroOne = shiftedVec / glm::vec2(WINDOW_WIDTH, WINDOW_HEIGHT);
		glm::vec2 flippedY = glm::vec2(scaledToZeroOne.x, 1.0f - scaledToZeroOne.y);
		glm::vec2 final = flippedY * 2.0f - glm::vec2(1.0f, 1.0f);

		input.cursorPos = glm::vec3(final, 0.0f);

		// Log::info("CursorPosCallback: xpos={}, ypos={}", input.cursorPos.x, input.cursorPos.y);
	}

	virtual void scrollCallback(double xoffset, double yoffset) override {
		Log::info("ScrollCallback: xoffset={}, yoffset={}", xoffset, yoffset);
	}

	virtual void windowSizeCallback(int width, int height) override {
		Log::info("WindowSizeCallback: width={}, height={}", width, height);
		CallbackInterface::windowSizeCallback(width, height); // Important, calls glViewport(0, 0, width, height);
	}

	CurveEditorCallbackInput getCurveEditorCallbackInput() {
		return input;
	}

private:
	CurveEditorCallbackInput input;
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
		: curveType(BEZIER), programComboSelection(0), pointComboSelection(0)
	{
		// Initialize options for the program combo box
		programOptions[0] = "Curve Editor";
		programOptions[1] = "Orbit Viewer";
		programOptions[2] = "Option 3";

		// Initialize options for the point mode combo box
		pointOptions[0] = "Select Mode";
		pointOptions[1] = "Insert Mode";
		pointOptions[2] = "Delete Mode";

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
		// ImGui::InputText("Input Text", inputText, IM_ARRAYSIZE(inputText));
		// Display the input text
		// ImGui::Text("You entered: %s", inputText);

		// Add spacing ------------------------------
		ImGuiAddSpace();

		// Combo box
		ImGui::Combo("Program Select", &programComboSelection, programOptions, IM_ARRAYSIZE(programOptions));
		
		// Add spacing ------------------------------
		ImGuiAddSpace();

		// Curve select buttons
		ImGui::Text("Select Curve Type:");
		ImGui::SameLine();
		if (ImGui::Button("Bezier")) {
			input.curveType = BEZIER;
		}
		ImGui::SameLine();
		if (ImGui::Button("B-Spline")) {
			input.curveType = B_SPLINE;
		}
		if (input.curveType == BEZIER) {
			ImGui::Text("Current Type: Bezier Curve");
		}
		else {
			ImGui::Text("Current Type: B-Spline Curve");
		}

		// Add spacing ------------------------------
		ImGuiAddSpace();

		ImGui::Combo("Point Mode Select", &pointComboSelection, pointOptions, IM_ARRAYSIZE(pointOptions));
		input.pointMode = static_cast<POINT_MODE>(pointComboSelection);

		if (ImGui::Button("Reset Points")) {
			resetPoints = true;
		}

		// Add spacing ------------------------------
		ImGuiAddSpace();

		// Reset Camera
		if (ImGui::Button("Reset Camera")) {
			// TODO
		}
	
		
		/* Old stuff
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
		*/
	}

	glm::vec3 getColor() const {
		return glm::vec3(colorValue[0], colorValue[1], colorValue[2]);
	}

	void ImGuiAddSpace() {
		ImGui::Spacing();
		ImGui::Separator();
		ImGui::Spacing();
	}

	/*-------------------------------------------------------*/
	enum CURVE_TYPE getCurveType() { return curveType; }
	CurveEditorPanelInput getCurveEditorPanelInput() { return input;  }

private:
	float colorValue[3];  // Array for RGB color values

	enum CURVE_TYPE curveType;

	const char* programOptions[3]; // Options for the program combo box
	int programComboSelection;

	const char* pointOptions[3]; // Options for the point combo box
	int pointComboSelection;

	CurveEditorPanelInput input;

};

/*------------------------ de Casteljau Algorithm -----------------------*/
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

/*----------------- Chaikin Curve Subdivision Algorithm -----------------*/
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

/*--------------------------- Extra Functions ---------------------------*/
int selectControlPoint(std::vector<glm::vec3>& controlPoints, glm::vec3 cursorPos, bool mousePress) {
	if (!mousePress || controlPoints.empty()) {
		return -1;
	}

	for (int i = 0; i < controlPoints.size(); i++) {
		float distance = glm::length(cursorPos - controlPoints[i]);

		if (distance <= POINT_PROXIMITY_THRESHOLD) {
			return i;
		}
	}

	return -1;
}

int main() {
	Log::debug("Starting main");

	// WINDOW
	glfwInit();
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE); // Make the window non-resizable
	Window window(WINDOW_WIDTH, WINDOW_HEIGHT, "CPSC 453: Assignment 3");
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
		{-.5f, -.5f, 0.f},
		{ .5f, -.5f, 0.f},
		{ .5f,  .5f, 0.f},
		{-.5f,  .5f, 0.f}
	};

	glm::vec3 cp_point_colour	= { 1.f,0.f,0.f };
	glm::vec3 cp_line_colour	= { 0.f,1.f,0.f };

	/*-------------- Geometry --------------*/
	CPU_Geometry cp_point_cpu;
	GPU_Geometry cp_point_gpu;

	/*-------------- Control Point Line --------------*/
	CPU_Geometry cp_line_cpu;
	GPU_Geometry cp_line_gpu;

	// curve geometry
	CPU_Geometry curve_cpu_geom;
	GPU_Geometry curve_gpu_geom;	


	while (!window.shouldClose()) {
		glfwPollEvents();
		CurveEditorCallbackInput callbackInput = curve_editor_callback->getCurveEditorCallbackInput();
		CurveEditorPanelInput panelInput = curve_editor_panel_renderer->getCurveEditorPanelInput();
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



		/*------------------------ Control points  ------------------------*/
		// Reset points
		if (resetPoints) {
			resetPoints = false;
			cp_positions_vector.clear();
		}

		// Select a control point
		int selectedControlPoint = selectControlPoint(cp_positions_vector, callbackInput.cursorPos, callbackInput.mousePress);

		switch (panelInput.pointMode) {
		case SELECT_MODE:
			if (selectedControlPoint != -1) {
				cp_positions_vector[selectedControlPoint] = callbackInput.cursorPos;
			}
			break;
		case INSERT_MODE:
			if (callbackInput.mousePress && letGo) {
				glm::vec3 newPoint = callbackInput.cursorPos;
				cp_positions_vector.push_back(newPoint);
				letGo = false;
				std::cout << "point added" << std::endl;
			}
			break;
		case DELETE_MODE:
			if (selectedControlPoint != -1) {
				cp_positions_vector.erase(cp_positions_vector.begin() + selectedControlPoint);
			}
			break;
		default:
			break;
		}
		/*---------------------- Control points end ----------------------*/

		/* ----------------- Curve -----------------*/
		// Curve points
		std::vector<glm::vec3> curve_points;

		// Generate points on the curve
		if (!cp_positions_vector.empty()) {
			switch (panelInput.curveType) {
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
		}
		/* ----------------- Curve End -----------------*/

		// Only render if there are points ---------------------------------------------------
		if (!cp_positions_vector.empty()) {
			// Control Point ----------------
			cp_point_cpu.verts = cp_positions_vector;
			cp_point_cpu.cols = std::vector<glm::vec3>(cp_point_cpu.verts.size(), cp_point_colour);
			cp_point_gpu.setVerts(cp_point_cpu.verts);
			cp_point_gpu.setCols(cp_point_cpu.cols);

			cp_point_gpu.bind();
			glPointSize(15.f);
			glDrawArrays(GL_POINTS, 0, cp_point_cpu.verts.size());

			// Control Point Line ------------
			cp_line_cpu.verts = cp_positions_vector; // We are using GL_LINE_STRIP (change this if you want to use GL_LINES)
			cp_line_cpu.cols = std::vector<glm::vec3>(cp_point_cpu.verts.size(), cp_line_colour);
			cp_line_gpu.setVerts(cp_line_cpu.verts);
			cp_line_gpu.setCols(cp_line_cpu.cols);

			cp_line_gpu.bind();
			//glLineWidth(10.f); //May do nothing (like it does on my computer): https://community.khronos.org/t/3-0-wide-lines-deprecated/55426
			glDrawArrays(GL_LINE_STRIP, 0, cp_line_cpu.verts.size());


			// Update CPU and GPU geometries
			curve_cpu_geom.verts = curve_points;
			curve_cpu_geom.cols = std::vector<glm::vec3>(curve_points.size(), { 0.0f, 0.0f, 0.0f }); // black curve line
			curve_gpu_geom.setVerts(curve_cpu_geom.verts);
			curve_gpu_geom.setCols(curve_cpu_geom.cols);

			// Bind and draw the curve
			curve_gpu_geom.bind();
			glDrawArrays(GL_LINE_STRIP, 0, curve_cpu_geom.verts.size());

		}

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
