#include <glad/glad.h>

#include <iostream>
#include <string>
#include <list>
#include <vector>
#include <limits>
#include <functional>
#include "cmath"
#include "corecrt_math_defines.h"

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
#define WINDOW_HEIGHT 1000
#define WINDOW_WIDTH 1200

#define POINT_PROXIMITY_THRESHOLD 0.08f

enum CURVE_TYPE {
	BEZIER,
	B_SPLINE,
};

enum PROGRAM_MODE {
	CURVE_EDITOR,
	ORBIT_VIEWER,
	SURFACE_OF_REVOLUTION,
	TENSOR
};

enum POINT_MODE {
	SELECT_MODE,
	INSERT_MODE,
	DELETE_MODE
};

/*------------------------------------ Structs ------------------------------------*/
struct CurveEditorCallbackInput
{
	glm::vec3 cursorPos = glm::vec3(0.0f, 0.0f, 0.5f);
	int scrollWheel = 0;
	bool mousePress = false;
};
CurveEditorCallbackInput curveEditorInput;

struct CurveEditorPanelInput
{
	enum CURVE_TYPE curveType = BEZIER;
	enum PROGRAM_MODE programMode = CURVE_EDITOR;
	enum POINT_MODE pointMode = SELECT_MODE;

	bool renderControlPoints = true;
	bool renderControlPointLines = true;

	bool wireframeMode = true;

	int tensorMode = 1;

	int sorSlices = 21;
	int sorIterations = 5;

	int tensorIterations = 3;
};
CurveEditorPanelInput curveEditorPanelInput;
/*--------------------------------- Global Static ---------------------------------*/
static bool letGo;
static bool resetPoints = false;
static bool resetCamera = false;

static glm::vec3 cameraPosition(5.0f, 5.0f, 5.0f);

const glm::mat4 projection = glm::perspective(
	glm::radians(45.0f),
	static_cast<float>(WINDOW_WIDTH / WINDOW_HEIGHT),
	0.1f, 100.0f
);

const std::vector<std::vector<glm::vec3>> tensorSurface1 = {
	{ glm::vec3(-2, 0, -2), glm::vec3(-1, 0, -2), glm::vec3(0,  0, -2), glm::vec3(1, 0, -2), glm::vec3(2, 0, -2) },
	{ glm::vec3(-2, 0, -1), glm::vec3(-1, 1, -1), glm::vec3(0,  1, -1), glm::vec3(1, 1, -1), glm::vec3(2, 0, -1) },
	{ glm::vec3(-2, 0,  0), glm::vec3(-1, 1,  0), glm::vec3(0, -1,  0), glm::vec3(1, 1,  0), glm::vec3(2, 0,  0) },
	{ glm::vec3(-2, 0,  1), glm::vec3(-1, 1,  1), glm::vec3(0,  1,  1), glm::vec3(1, 1,  1), glm::vec3(2, 0,  1) },
	{ glm::vec3(-2, 0,  2), glm::vec3(-1, 0,  2), glm::vec3(0,  0,  2), glm::vec3(1, 0,  2), glm::vec3(2, 0,  2) }
};

const std::vector<std::vector<glm::vec3>> tensorSurface2 = {
	{ glm::vec3(-2, 0.3, -2), glm::vec3(-1, 1.2, -2), glm::vec3(0,  0.8, -2), glm::vec3(1, 0.1, -2), glm::vec3(2, 0.6, -2) },
	{ glm::vec3(-2, 1.0, -1), glm::vec3(-1, 0.5, -1), glm::vec3(0,  1.8, -1), glm::vec3(1, 1.3, -1), glm::vec3(2, 0.4, -1) },
	{ glm::vec3(-2, 0.2,  0), glm::vec3(-1, 1.6,  0), glm::vec3(0, -0.1,  0), glm::vec3(1, 1.1,  0), glm::vec3(2, 0.7,  0) },
	{ glm::vec3(-2, 1.5,  1), glm::vec3(-1, 0.9,  1), glm::vec3(0,  1.2,  1), glm::vec3(1, 0.3,  1), glm::vec3(2, 0.1,  1) },
	{ glm::vec3(-2, 0.7,  2), glm::vec3(-1, 1.0,  2), glm::vec3(0,  0.5,  2), glm::vec3(1, 1.7,  2), glm::vec3(2, 0.8,  2) }
};

class CurveEditorCallBack : public CallbackInterface {
public:
	CurveEditorCallBack() {}

	virtual void keyCallback(int key, int scancode, int action, int mods) override {
		Log::info("KeyCallback: key={}, action={}", key, action);
	}

	virtual void mouseButtonCallback(int button, int action, int mods) override {
		if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
			curveEditorInput.mousePress = true;
		}
		else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
			curveEditorInput.mousePress = false;
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
		curveEditorInput.cursorPos = glm::vec3(final, 0.0f);

		// Log::info("CursorPosCallback: xpos={}, ypos={}", input.cursorPos.x, input.cursorPos.y);
	}

	virtual void scrollCallback(double xoffset, double yoffset) override {
		Log::info("ScrollCallback: xoffset={}, yoffset={}", xoffset, yoffset);
	}

	virtual void windowSizeCallback(int width, int height) override {
		Log::info("WindowSizeCallback: width={}, height={}", width, height);
		CallbackInterface::windowSizeCallback(width, height); // Important, calls glViewport(0, 0, width, height);
	}

private:
	
};

// Can swap the callback instead of maintaining a state machine

class TurnTable3DViewerCallBack : public CallbackInterface {

public:
	TurnTable3DViewerCallBack()
		: dragCamera(false), lastCursorPos(0.0f), yaw(90.0f), pitch(0.0f), distance(3.0f), sensitivity(0.1f) {}

	virtual void keyCallback(int key, int scancode, int action, int mods) {}
	virtual void mouseButtonCallback(int button, int action, int mods) {
		if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
			dragCamera = true;
		}
		else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
			dragCamera = false;
		}
	}
	virtual void cursorPosCallback(double xpos, double ypos) {
		glm::vec2 currentCursorPos(xpos, ypos);

		if (dragCamera) {
			glm::vec2 offset = currentCursorPos - lastCursorPos;
			offset *= sensitivity;

			// Rotations
			yaw += offset.x;
			pitch += offset.y;

			// Clamp pitch to avoid flipping
			pitch = glm::clamp(pitch, -89.0f, 89.0f);

			// Convert spherical coordinates to Cartesian coordinates
			float x = distance * cos(glm::radians(pitch)) * cos(glm::radians(yaw));
			float y = distance * sin(glm::radians(pitch));
			float z = distance * cos(glm::radians(pitch)) * sin(glm::radians(yaw));

			cameraPosition = glm::vec3(x, y, z);
		}

		lastCursorPos = currentCursorPos;
	}
	virtual void scrollCallback(double xoffset, double yoffset) {
		// Adjust the distance (radius) using the scroll wheel
		distance -= yoffset * sensitivity;
		distance = glm::clamp(distance, 0.1f, 10.0f); // Prevent zooming too close or too far

		// Convert spherical coordinates to Cartesian coordinates
		float x = distance * cos(glm::radians(pitch)) * cos(glm::radians(yaw));
		float y = distance * sin(glm::radians(pitch));
		float z = distance * cos(glm::radians(pitch)) * sin(glm::radians(yaw));

		cameraPosition = glm::vec3(x, y, z);

	}
	virtual void windowSizeCallback(int width, int height) {
		// The CallbackInterface::windowSizeCallback will call glViewport for us
		CallbackInterface::windowSizeCallback(width, height);
	}

	glm::vec3 getCameraPosition() {
		return cameraPosition;
	}

	void resetCameraPosition() {
		cameraPosition = glm::vec3(0.0f, 0.0f, 3.0f);
		lastCursorPos = glm::vec2(0.0f);
		yaw = 90.0f;
		pitch = 0.0f;
		distance = 3.0f;
	}

private:
	bool dragCamera = false;
	float sensitivity;

	float yaw;      // X-axis angle
	float pitch;    // Y-axis angle
	float distance;   // Distance of camera from the origin

	glm::vec2 lastCursorPos = glm::vec2(0.0f, 0.0f);
	glm::vec3 cameraPosition = glm::vec3(0.0f, 0.0f, 3.0f);
};


class CurveEditorPanelRenderer : public PanelRendererInterface {
public:
	CurveEditorPanelRenderer()
		: curveType(BEZIER), programComboSelection(0), pointComboSelection(0)
	{
		// Initialize options for the program combo box
		programOptions[0] = "Curve Editor";
		programOptions[1] = "Orbit Viewer";
		programOptions[2] = "Surface of Revolution";
		programOptions[3] = "Tensor Product Surface";

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
		curveEditorPanelInput.programMode = static_cast<PROGRAM_MODE>(programComboSelection);

		// Curve select buttons
		if ((curveEditorPanelInput.programMode == CURVE_EDITOR) || (curveEditorPanelInput.programMode == ORBIT_VIEWER)) {
			// Add spacing ------------------------------
			ImGuiAddSpace();
			ImGui::Text("Select Curve Type:");
			ImGui::SameLine();
			if (ImGui::Button("Bezier")) {
				curveEditorPanelInput.curveType = BEZIER;
			}
			ImGui::SameLine();
			if (ImGui::Button("B-Spline")) {
				curveEditorPanelInput.curveType = B_SPLINE;
			}
			if (curveEditorPanelInput.curveType == BEZIER) {
				ImGui::Text("Current Type: Bezier Curve");
			}
			else {
				ImGui::Text("Current Type: B-Spline Curve");
			}
		}

		ImGuiAddSpace();

		if (curveEditorPanelInput.programMode == CURVE_EDITOR) {
			// Add spacing ------------------------------
			ImGui::Combo("Point Mode Select", &pointComboSelection, pointOptions, IM_ARRAYSIZE(pointOptions));
			curveEditorPanelInput.pointMode = static_cast<POINT_MODE>(pointComboSelection);
		}

		if (curveEditorPanelInput.programMode != TENSOR) {
			ImGui::Checkbox("Render Control Points", &curveEditorPanelInput.renderControlPoints);
		}

		ImGui::Checkbox("Render Control Point Lines", &curveEditorPanelInput.renderControlPointLines);

		if (curveEditorPanelInput.programMode == CURVE_EDITOR) {
			// Reset Points Button
			if (ImGui::Button("Reset Points")) {
				resetPoints = true;
			}
		}

		if ((curveEditorPanelInput.programMode == SURFACE_OF_REVOLUTION) || (curveEditorPanelInput.programMode == TENSOR)) {
			// Checkbox
			ImGui::Checkbox("Wireframe Mode", &curveEditorPanelInput.wireframeMode);

			// Sliders for Surface of Revolution and Tensor Mode
			if (curveEditorPanelInput.programMode == SURFACE_OF_REVOLUTION) {
				ImGuiAddSpace();
				ImGui::Text("Adjust Surface Parameters");
				ImGui::SliderInt("Revolution Slices", &curveEditorPanelInput.sorSlices, 5, 30, "%d slices");
				ImGui::SliderInt("B-Spline Iterations", &curveEditorPanelInput.sorIterations, 3, 10, "%d iterations");
			}
			else if (curveEditorPanelInput.programMode == TENSOR) {
				ImGuiAddSpace();
				ImGui::Text("Select Tensor Product Surface");
				if (ImGui::Button("Default")) {
					curveEditorPanelInput.tensorMode = 1;
				}
				ImGui::SameLine();
				if (ImGui::Button("My Tensor")) {
					curveEditorPanelInput.tensorMode = 2;
				}

				ImGuiAddSpace();
				ImGui::SliderInt("Tensor Iterations", &curveEditorPanelInput.tensorIterations, 1, 5, "%d iterations");
			}
		}

		// Add spacing ------------------------------
		ImGuiAddSpace();

		// Reset Camera
		if (ImGui::Button("Reset Camera")) {
			resetCamera = true;
		}
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

private:
	float colorValue[3];  // Array for RGB color values

	enum CURVE_TYPE curveType;

	const char* programOptions[4]; // Options for the program combo box
	int programComboSelection;

	const char* pointOptions[3]; // Options for the point combo box
	int pointComboSelection;
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

/*-------------------- Generate Surface of Revolution -------------------*/
std::vector<glm::vec3> surfaceOfRevolution(const std::vector<glm::vec3>& curvePoints, int n_slices = 21) {
	std::vector<glm::vec3> surface_verts;

	// Angle between each curve
	float angle_step = 2.0f * M_PI / n_slices;

	// For each curve point, create triangles with adjacent curve points
	for (int i = 0; i < curvePoints.size() - 1; i++) {
		glm::vec3 p1 = curvePoints[i];
		glm::vec3 p2 = curvePoints[i + 1];

		for (int j = 0; j < n_slices; j++) {
			float angle = j * angle_step;
			float next_angle = (j + 1) * angle_step;

			glm::vec3 v1(p1.x * cos(angle), p1.y, p1.x * sin(angle));
			glm::vec3 v2(p2.x * cos(angle), p2.y, p2.x * sin(angle));
			glm::vec3 v3(p2.x * cos(next_angle), p2.y, p2.x * sin(next_angle));
			glm::vec3 v4(p1.x * cos(next_angle), p1.y, p1.x * sin(next_angle));

			surface_verts.push_back(v1);
			surface_verts.push_back(v2);
			surface_verts.push_back(v3);
			surface_verts.push_back(v1);
			surface_verts.push_back(v3);
			surface_verts.push_back(v4);
		}
	}

	return surface_verts;
}

/*------------------ Generate Tensor Product Surface -------------------*/
std::vector<glm::vec3> tensorProductSurface(std::vector<std::vector<glm::vec3>>& controlPoints, int iterations = 3) {

	// Smooth each row by applying chaikin algorithm
	std::vector<std::vector<glm::vec3>> smoothedRows;
	for (const std::vector<glm::vec3>& row : controlPoints) {
		std::vector<glm::vec3> coarseRow = row;
		smoothedRows.push_back(chaikinCurveSubdivision(coarseRow, iterations));
	}

	// Transpose smoothed grid
	std::vector<std::vector<glm::vec3>> transposedGrid;
	for (int i = 0; i < smoothedRows[0].size(); i++) {
		std::vector<glm::vec3> column;
		for (std::vector<glm::vec3>& row : smoothedRows) {
			column.push_back(row[i]);
		}
		transposedGrid.push_back(column);
	}

	// Smooth each column (rows in transposed grid)
	std::vector<std::vector<glm::vec3>> smoothedColumns;
	for (const std::vector<glm::vec3>& column : transposedGrid) {
		std::vector<glm::vec3> col = column;
		smoothedColumns.push_back(chaikinCurveSubdivision(col, iterations));
	}

	// Transpose grid back to original layout
	std::vector<std::vector<glm::vec3>> finalGrid;
	for (int i = 0; i < smoothedColumns[0].size(); i++) {
		std::vector<glm::vec3> row;
		for (const auto& col : smoothedColumns) {
			row.push_back(col[i]);
		}
		finalGrid.push_back(row);
	}

	// Generate triangles
	std::vector<glm::vec3> surfacePoints;
	for (int i = 0; i < finalGrid.size() - 1; i++) {
		for (int j = 0; j < finalGrid[i].size() - 1; j++) {
			glm::vec3 v1 = finalGrid[i][j];
			glm::vec3 v2 = finalGrid[i + 1][j];
			glm::vec3 v3 = finalGrid[i + 1][j + 1];
			glm::vec3 v4 = finalGrid[i][j + 1];

			surfacePoints.push_back(v1);
			surfacePoints.push_back(v2);
			surfacePoints.push_back(v3);

			surfacePoints.push_back(v1);
			surfacePoints.push_back(v3);
			surfacePoints.push_back(v4);
		}
	}

	return surfacePoints;
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
	auto turn_table_3D_viewer_callback = std::make_shared<TurnTable3DViewerCallBack>();
	auto curve_editor_panel_renderer = std::make_shared<CurveEditorPanelRenderer>();

	//Set callback to window
	// window.setCallbacks(curve_editor_callback);
	// Can swap the callback instead of maintaining a state machine
	// window.setCallbacks(turn_table_3D_viewer_callback);

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

	// Surface geometry
	CPU_Geometry surface_cpu_geom;
	GPU_Geometry surface_gpu_geom;

	// Tensor geometry
	CPU_Geometry tensor_cpu_geom;
	GPU_Geometry tensor_gpu_geom;

	while (!window.shouldClose()) {

		// Use callback for either 2D editor or 3d viewer
		switch (curveEditorPanelInput.programMode) {
		case CURVE_EDITOR:
			window.setCallbacks(curve_editor_callback);
			break;
		default:
			window.setCallbacks(turn_table_3D_viewer_callback);
			break;
		}

		glfwPollEvents();
		CurveEditorPanelInput panelInput = curveEditorPanelInput;
		CurveEditorCallbackInput callbackInput = curveEditorInput;
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

		// Reset points
		if (resetPoints) {
			resetPoints = false;
			cp_positions_vector.clear();
		}

		// Reset camera
		if (resetCamera) {
			resetCamera = false;
			turn_table_3D_viewer_callback->resetCameraPosition();
		}

		// View Projection Matrix
		glm::mat4 viewProjection;

		// selected Control point initialized to -1
		int selectedControlPoint =  -1;

		// Curve points
		std::vector<glm::vec3> curve_points;

		// Surface of revolution verts
		std::vector<glm::vec3> surface_verts;

		// Tensor Points
		std::vector<std::vector<glm::vec3>> tensorPoints;
		if (curveEditorPanelInput.tensorMode == 1) {
			tensorPoints = tensorSurface1;
		}
		else {
			tensorPoints = tensorSurface2;
		}

		/*----------------------------------------------------------- 3D View or 2D Point Editor-----------------------------------------------------------*/
		switch (panelInput.programMode) {
		case CURVE_EDITOR:
			// Default view projection
			viewProjection = glm::mat4(1.0f);

			/*------------------------ Control points  ------------------------*/
			// Select a control point
			selectedControlPoint = selectControlPoint(cp_positions_vector, callbackInput.cursorPos, callbackInput.mousePress);
			switch (panelInput.pointMode) {
			case SELECT_MODE:
				if (selectedControlPoint != -1) {
					cp_positions_vector[selectedControlPoint] = callbackInput.cursorPos;
				}
				break;
			case INSERT_MODE:
				if (callbackInput.mousePress && letGo && (cp_positions_vector.size() < 12)) {
					glm::vec3 newPoint = callbackInput.cursorPos;
					cp_positions_vector.push_back(newPoint);
					letGo = false;
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
			break;
		default:
			glm::vec3 camPos = turn_table_3D_viewer_callback->getCameraPosition();
			glm::mat4 view = glm::lookAt(camPos, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			viewProjection = projection * view;
			break;
		}

		// Send new-projection matrix to vertex shader
		glUniformMatrix4fv(
			glGetUniformLocation(shader_program_default.getProgram(), "transformationMatrix"),
			1, GL_FALSE, glm::value_ptr(viewProjection)
		);

		/*----------------------------------------------------------- Scene Type -----------------------------------------------------------*/
		switch (panelInput.programMode) {
		case SURFACE_OF_REVOLUTION:
			// Calculate surface points
			surface_cpu_geom.verts.clear();
			surface_cpu_geom.verts = surfaceOfRevolution(chaikinCurveSubdivision(cp_positions_vector, curveEditorPanelInput.sorIterations), curveEditorPanelInput.sorSlices);
			surface_cpu_geom.cols = std::vector<glm::vec3>(surface_cpu_geom.verts.size(), glm::vec3(0.f, 0.f, 0.f));

			// Render Surface
			surface_gpu_geom.setVerts(surface_cpu_geom.verts);
			surface_gpu_geom.setCols(surface_cpu_geom.cols);

			// Wireframe mode on/off
			if (panelInput.wireframeMode) {
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			}
			else {
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			}
			surface_gpu_geom.bind();
			glDrawArrays(GL_TRIANGLES, 0, surface_cpu_geom.verts.size());
			break;
		case TENSOR:
			tensor_cpu_geom.verts.clear();
			tensor_cpu_geom.verts = tensorProductSurface(tensorPoints, curveEditorPanelInput.tensorIterations);
			tensor_cpu_geom.cols = std::vector<glm::vec3>(tensor_cpu_geom.verts.size(), glm::vec3(0.f, 0.f, 0.f));

			tensor_gpu_geom.setVerts(tensor_cpu_geom.verts);
			tensor_gpu_geom.setCols(tensor_cpu_geom.cols);

			// Wireframe mode on/off
			if (panelInput.wireframeMode) {
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			}
			else {
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			}

			tensor_gpu_geom.bind();
			glDrawArrays(GL_TRIANGLES, 0, tensor_cpu_geom.verts.size());
			break;
		default:
			// Calculate curve points
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

			// Render Curve
			curve_cpu_geom.verts = curve_points;
			curve_cpu_geom.cols = std::vector<glm::vec3>(curve_points.size(), { 0.0f, 0.0f, 0.0f }); // black curve line
			curve_gpu_geom.setVerts(curve_cpu_geom.verts);
			curve_gpu_geom.setCols(curve_cpu_geom.cols);
			curve_gpu_geom.bind();
			glDrawArrays(GL_LINE_STRIP, 0, curve_cpu_geom.verts.size());
			break;

			break;
		}


		/*------------------------------------------------------- Control Points and Control Point Lines -------------------------------------------------------*/
		switch (curveEditorPanelInput.programMode) {
		case TENSOR:
			// Tensor Product Surface Control Point Lines
			if (panelInput.renderControlPointLines) {
				cp_line_cpu.verts.clear();
				cp_line_cpu.cols.clear();

				// Draw Horizontal Lines (connect adjacent control points in each row)
				for (int i = 0; i < tensorPoints.size(); i++) {
					for (int j = 0; j < tensorPoints[i].size() - 1; j++) {
						cp_line_cpu.verts.push_back(tensorPoints[i][j]);
						cp_line_cpu.verts.push_back(tensorPoints[i][j + 1]);
						cp_line_cpu.cols.push_back(cp_line_colour);
						cp_line_cpu.cols.push_back(cp_line_colour);
					}
				}

				// Draw Vertical Lines (connect adjacent control points in each column)
				for (int j = 0; j < tensorPoints[0].size(); j++) {
					for (int i = 0; i < tensorPoints.size() - 1; i++) {
						cp_line_cpu.verts.push_back(tensorPoints[i][j]);
						cp_line_cpu.verts.push_back(tensorPoints[i + 1][j]);
						cp_line_cpu.cols.push_back(cp_line_colour);
						cp_line_cpu.cols.push_back(cp_line_colour);
					}
				}

				cp_line_gpu.setVerts(cp_line_cpu.verts);
				cp_line_gpu.setCols(cp_line_cpu.cols);
				cp_line_gpu.bind();
				glDrawArrays(GL_LINES, 0, cp_line_cpu.verts.size());
			}
			break;
		default:
			if (!cp_positions_vector.empty()) {
				// Control Point ----------------
				if (panelInput.renderControlPoints) { // Optional: render/unrender cp
					cp_point_cpu.verts = cp_positions_vector;
					cp_point_cpu.cols = std::vector<glm::vec3>(cp_point_cpu.verts.size(), cp_point_colour);
					cp_point_gpu.setVerts(cp_point_cpu.verts);
					cp_point_gpu.setCols(cp_point_cpu.cols);

					cp_point_gpu.bind();
					glPointSize(15.f);
					glDrawArrays(GL_POINTS, 0, cp_point_cpu.verts.size());
				}

				// Control Point Line ------------
				if (panelInput.renderControlPointLines) { // Optional: render/unrender cp lines
					cp_line_cpu.verts = cp_positions_vector; // We are using GL_LINE_STRIP (change this if you want to use GL_LINES)
					cp_line_cpu.cols = std::vector<glm::vec3>(cp_point_cpu.verts.size(), cp_line_colour);
					cp_line_gpu.setVerts(cp_line_cpu.verts);
					cp_line_gpu.setCols(cp_line_cpu.cols);

					cp_line_gpu.bind();
					//glLineWidth(10.f); //May do nothing (like it does on my computer): https://community.khronos.org/t/3-0-wide-lines-deprecated/55426
					glDrawArrays(GL_LINE_STRIP, 0, cp_line_cpu.verts.size());
				}
			}
			break;
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
