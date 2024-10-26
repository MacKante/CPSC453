#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>

#define _USE_MATH_DEFINES
#include <math.h>

#include "Geometry.h"
#include "GLDebug.h"
#include "Log.h"
#include "ShaderProgram.h"
#include "Shader.h"
#include "Texture.h"
#include "Window.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include <glm/gtc/type_ptr.hpp>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 800

#define DEFAULT_SHIP_WIDTH 0.12f
#define DEFAULT_SHIP_HEIGHT 0.08f

#define DEFAULT_DIAMOND_WIDTH 0.10f
#define DEFAULT_DIAMOND_HEIGHT 0.10f


// An example struct for Game Objects.
// You are encouraged to customize this as you see fit.
struct GameObject {
	// Struct's constructor deals with the texture.
	// Also sets default position, theta, scale, and transformationMatrix
	GameObject(std::string texturePath, GLenum textureInterpolation, float objectWidth, float objectHeight) :
		texture(texturePath, textureInterpolation)
	{
		// vertex coordinates
		cgeom.verts.push_back(glm::vec3(-1.f, 1.f, 0.f));
		cgeom.verts.push_back(glm::vec3(-1.f, -1.f, 0.f));
		cgeom.verts.push_back(glm::vec3(1.f, -1.f, 0.f));
		cgeom.verts.push_back(glm::vec3(-1.f, 1.f, 0.f));
		cgeom.verts.push_back(glm::vec3(1.f, -1.f, 0.f));
		cgeom.verts.push_back(glm::vec3(1.f, 1.f, 0.f));

		// texture coordinates
		cgeom.texCoords.push_back(glm::vec2(0.f, 1.f));
		cgeom.texCoords.push_back(glm::vec2(0.f, 0.f));
		cgeom.texCoords.push_back(glm::vec2(1.f, 0.f));
		cgeom.texCoords.push_back(glm::vec2(0.f, 1.f));
		cgeom.texCoords.push_back(glm::vec2(1.f, 0.f));
		cgeom.texCoords.push_back(glm::vec2(1.f, 1.f));

		// GPU Geometry
		ggeom.setVerts(cgeom.verts);
		ggeom.setTexCoords(cgeom.texCoords);

		// Setting Initial Transformation Matrixes
		scalingMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(objectWidth, objectHeight, 1.0f));
		rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		translationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	}

	glm::vec3 getPosition() {
		return glm::vec3(translationMatrix[3][0], translationMatrix[3][1], 1.0f);
	}

	glm::mat4 getTransformationMatrix() {
		return translationMatrix * rotationMatrix * scalingMatrix;
	}

	void translateObject(glm::vec3 translateVec) {
		translationMatrix = glm::translate(translationMatrix, translateVec);
	}

	void updateShip(glm::vec2 cursorPosition, glm::vec3 translateVector) {

	}

	void updateDiamond() {

	}

	CPU_Geometry cgeom;
	GPU_Geometry ggeom;
	Texture texture;

	glm::vec3 position;

	glm::mat4 scalingMatrix;
	glm::mat4 rotationMatrix;
	glm::mat4 translationMatrix;
};

// EXAMPLE CALLBACKS
class MyCallbacks : public CallbackInterface {

public:
	MyCallbacks(ShaderProgram& shader, int screenWidth, int screenHeight) :
		screenDim(screenWidth, screenHeight),
		shader(shader)
	{}

	virtual void keyCallback(int key, int scancode, int action, int mods) {
		if (key == GLFW_KEY_R && action == GLFW_PRESS) {
			resetFlag = true;
			shader.recompile();
		}
		else if (key == GLFW_KEY_W && action == GLFW_PRESS) {
			playerInput = glm::vec3(0.0f, 0.10f, 0.0f);
			// std::cout << "X coordinate: "<< transformationMatrix[3][0] << " Y coordinate: " << transformationMatrix[3][1] << std::endl;
			// std::cout << "Forward key pressed" << std::endl;
		}
		else if (key == GLFW_KEY_S && action == GLFW_PRESS) {
			playerInput = glm::vec3(0.0f, -0.10f, 0.0f);
			// std::cout << "X coordinate: " << transformationMatrix[3][0] << " Y coordinate: " << transformationMatrix[3][1] << std::endl;
			// std::cout << "Backward key pressed" << std::endl;
		}
	}

	// Cursor Position Input
	glm::vec2 mouseGL() {
		glm::vec2 startingVec(xScreenPosition, yScreenPosition);
		glm::vec2 shiftedVec = startingVec + glm::vec2(0.5f, 0.5f);
		glm::vec2 scaledToZeroOne = shiftedVec / glm::vec2(screenDim);
		glm::vec2 flippedY = glm::vec2(scaledToZeroOne.x, 1.0f - scaledToZeroOne.y);
		glm::vec2 final = flippedY * 2.0f - glm::vec2(1.0f, 1.0f);

		return final;
	}

	virtual void cursorPosCallback(double xpos, double ypos) {
		xScreenPosition = xpos;
		yScreenPosition = ypos;
		std::cout << "xPos: " << mouseGL().x << ", " << "yPos: " << mouseGL().y << std::endl;
	}

	glm::vec3 getPlayerInput() {
		glm::vec3 returnInput = playerInput;
		playerInput = glm::vec3(0.0f, 0.0f, 0.0f);
		return returnInput;
	}
	
private:
	glm::ivec2 screenDim;

	double xScreenPosition;
	double yScreenPosition;

	glm::vec3 playerInput = glm::vec3(0.0f, 0.0f, 0.0f);
	bool resetFlag = false;

	ShaderProgram& shader;
};

// END EXAMPLES

int main() {
	Log::debug("Starting main");

	// WINDOW
	glfwInit();
	Window window(WINDOW_WIDTH, WINDOW_HEIGHT, "CPSC 453"); // can set callbacks at construction if desired

	GLDebug::enable();

	// SHADERS
	ShaderProgram shader("shaders/test.vert", "shaders/test.frag");

	// CALLBACKS
	std::shared_ptr<MyCallbacks> callback = std::make_shared<MyCallbacks>(shader, WINDOW_WIDTH, WINDOW_HEIGHT);
	window.setCallbacks(callback); // can also update callbacks to new ones

	// GL_NEAREST looks a bit better for low-res pixel art than GL_LINEAR.
	// But for most other cases, you'd want GL_LINEAR interpolation.

	GameObject ship("textures/ship.png", GL_NEAREST, DEFAULT_SHIP_WIDTH, DEFAULT_SHIP_HEIGHT);

	// Game Score
	int score = 0;

	// RENDER LOOP
	while (!window.shouldClose()) {
		
		glfwPollEvents();

		glm::vec3 playerInput = callback->getPlayerInput();
		ship.translateObject(playerInput);

		shader.use();

		glm::mat4 transformationMatrix = ship.getTransformationMatrix();

		glUniformMatrix4fv(
			glGetUniformLocation(shader.getProgram(), "transformationMatrix"),
			1, GL_FALSE, glm::value_ptr(transformationMatrix)
		);

		// std::cout << "X coordinate: " << transformationMatrix[3][0] << " Y coordinate: " << transformationMatrix[3][1] << std::endl;
		 
		ship.ggeom.bind();

		glEnable(GL_FRAMEBUFFER_SRGB);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		ship.texture.bind();
		glDrawArrays(GL_TRIANGLES, 0, 6);
		ship.texture.unbind();
		glDisable(GL_FRAMEBUFFER_SRGB); // disable sRGB for things like imgui

		// Starting the new ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		// Putting the text-containing window in the top-left of the screen.
		ImGui::SetNextWindowPos(ImVec2(5, 5));

		// Setting flags
		ImGuiWindowFlags textWindowFlags =
			ImGuiWindowFlags_NoMove |				// text "window" should not move
			ImGuiWindowFlags_NoResize |				// should not resize
			ImGuiWindowFlags_NoCollapse |			// should not collapse
			ImGuiWindowFlags_NoSavedSettings |		// don't want saved settings mucking things up
			ImGuiWindowFlags_AlwaysAutoResize |		// window should auto-resize to fit the text
			ImGuiWindowFlags_NoBackground |			// window should be transparent; only the text should be visible
			ImGuiWindowFlags_NoDecoration |			// no decoration; only the text should be visible
			ImGuiWindowFlags_NoTitleBar;			// no title; only the text should be visible

		// Begin a new window with these flags. (bool *)0 is the "default" value for its argument.
		ImGui::Begin("scoreText", (bool *)0, textWindowFlags);

		// Scale up text a little, and set its value
		ImGui::SetWindowFontScale(1.5f);
		ImGui::Text("Score: %d", score); // Second parameter gets passed into "%d"

		// End the window.
		ImGui::End();

		ImGui::Render();	// Render the ImGui window
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData()); // Some middleware thing

		window.swapBuffers();
	}
	// ImGui cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwTerminate();
	return 0;
}
