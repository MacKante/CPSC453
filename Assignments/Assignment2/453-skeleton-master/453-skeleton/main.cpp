#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime> 

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

// Player Input Struct
struct PlayerInput {
	glm::vec2 cursorPosition = glm::vec2(0.0f, 1.0f);
	bool startGame = false;
	bool resetFlag = false;
	bool isMovingForward = false;
	bool isMovingBackward = false;
};

// An example struct for Game Objects.
// You are encouraged to customize this as you see fit.
struct GameObject {
	// Struct's constructor deals with the texture.
	// Also sets default position, theta, scale, and transformationMatrix
	GameObject(std::string texturePath, GLenum textureInterpolation, float objectWidth, float objectHeight) :
		texture(texturePath, textureInterpolation),
		position(glm::vec2(0.0f, 0.0f)),
		direction(glm::vec2(0.0f, 0.0f)),
		appear(true)
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

	glm::mat4 getTransformationMatrix() {
		return translationMatrix * rotationMatrix * scalingMatrix;
	}

	void resizeShip(int score) {
		scalingMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(DEFAULT_SHIP_WIDTH + (0.05 * score), DEFAULT_SHIP_HEIGHT + (0.05 * score), 1.0f));
	}

	void updateShip(PlayerInput input) {
		// Check for reset flag
		if (input.resetFlag == true) {
			position = glm::vec2(0.0f, 0.0f);
			direction = glm::vec2(0.0f, 1.0f);
			scalingMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(DEFAULT_SHIP_WIDTH, DEFAULT_SHIP_HEIGHT, 1.0f));
			rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
			translationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
			return;
		}

		// Calculate direction vector and normalize
		glm::vec2 directionToCursor = input.cursorPosition - position;
		float distanceToCursor = glm::length(directionToCursor);

		// Compute the angle between the current direction and the new direction
		float angleToCursor = atan2(directionToCursor.y, directionToCursor.x) - glm::radians(90.0f); // Angle to face cursor

		// Set the rotation matrix to new angle
		rotationMatrix = glm::rotate(glm::mat4(1.0f), angleToCursor, glm::vec3(0.0f, 0.0f, 1.0f));

		//proximity threshold to prevent moving towards the cursor
		const float proximityThreshold = 0.15f; 

		// Check if the ship is far enough from the cursor
		if (distanceToCursor > proximityThreshold) {
			glm::vec2 newDirection = glm::normalize(directionToCursor);

			// Translate if moving forward or backward
			glm::vec3 translation(0.0f);
			if (input.isMovingForward && !input.isMovingBackward) {
				translation = glm::vec3(newDirection * 0.02f, 0.0f); 
			}
			else if (!input.isMovingForward && input.isMovingBackward) {
				translation = glm::vec3(-newDirection * 0.02f, 0.0f); 
			}

			// Calculate new position
			glm::vec2 newPosition = position + glm::vec2(translation.x, translation.y);

			// Check if new position is within bounds
			if (newPosition.x >= -1.0f + (DEFAULT_SHIP_WIDTH / 2) && newPosition.x <= 1.0f - (DEFAULT_SHIP_WIDTH / 2) &&
				newPosition.y >= -1.0f + (DEFAULT_SHIP_HEIGHT / 2) && newPosition.y <= 1.0f - (DEFAULT_SHIP_HEIGHT / 2)) {
				// If new position is within bounds, apply the translation
				translationMatrix = glm::translate(translationMatrix, translation);
				position = newPosition;
			}
		}
		else {
			std::cout << "Ship is too close to the cursor. Not moving." << std::endl;
		}
	}


	void initializeDiamond() {
		// Generate a random position within the window bounds
		appear = true;
		
		position = glm::vec2(
			(static_cast<float>(rand()) / RAND_MAX) - 0.5f,  // Random X position
			(static_cast<float>(rand()) / RAND_MAX) - 0.5f  // Random Y position
		);
		// std::cout << "X: " << position.x << " Y: " << position.y << std::endl;

		// Create a translation matrix using the position
		translationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(position, 0.0f));

		// Generate a random direction vector in the range of [-1, 1]
		direction = glm::vec2(
			(static_cast<float>(rand()) / RAND_MAX * 2.0f) - 1.0f, // Random X direction between -1 and 1
			(static_cast<float>(rand()) / RAND_MAX * 2.0f) - 1.0f  // Random Y direction between -1 and 1
		);

		// std::cout << "X: " << direction.x << " Y: " << direction.y << std::endl;

		// Normalize the direction vector to maintain consistent speed
		direction = glm::normalize(direction);
	}

	void updateDiamond(PlayerInput input) {
		// Update the position based on direction
		position += direction * 0.003f; // Move in the current direction

		// Check bounds and invert direction if needed
		if (position.x <= -1.0f || position.x >= 1.0f) {
			direction.x = -direction.x; // Invert X direction
		}
		if (position.y <= -1.0f || position.y >= 1.0f) {
			direction.y = -direction.y; // Invert Y direction
		}

		// Update the transformation matrix
		translationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(position, 0.0f));
	}

	CPU_Geometry cgeom;
	GPU_Geometry ggeom;
	Texture texture;

	bool appear;

	glm::vec2 position;
	glm::vec2 direction;

	glm::mat4 scalingMatrix;
	glm::mat4 rotationMatrix;
	glm::mat4 translationMatrix;
};

bool isCaught(const glm::vec2& shipPosition, const glm::vec2& diamondPosition) {
	float distance = glm::length(shipPosition - diamondPosition);
	return distance <= 0.080f;
}

// EXAMPLE CALLBACKS
class MyCallbacks : public CallbackInterface {

public:
	MyCallbacks(ShaderProgram& shader, int screenWidth, int screenHeight) :
		screenDim(screenWidth, screenHeight),
		shader(shader)
	{}

	virtual void keyCallback(int key, int scancode, int action, int mods) {
		if (key == GLFW_KEY_R && action == GLFW_PRESS) {
			shader.recompile();
			resetGame();
		}
		else if (key == GLFW_KEY_W) {
			playerInput.startGame = true;
			playerInput.isMovingForward = true;
			playerInput.isMovingBackward = false;
			// std::cout << "Forward key pressed" << std::endl;
		}
		else if (key == GLFW_KEY_S) {
			playerInput.startGame = true;
			playerInput.isMovingBackward = true;
			playerInput.isMovingForward = false;
			// std::cout << "Backward key pressed" << std::endl;
		}
	}

	virtual void cursorPosCallback(double xpos, double ypos) {
		glm::vec2 startingVec(xpos, ypos);
		glm::vec2 shiftedVec = startingVec + glm::vec2(0.5f, 0.5f);
		glm::vec2 scaledToZeroOne = shiftedVec / glm::vec2(screenDim);
		glm::vec2 flippedY = glm::vec2(scaledToZeroOne.x, 1.0f - scaledToZeroOne.y);
		glm::vec2 final = flippedY * 2.0f - glm::vec2(1.0f, 1.0f);

		playerInput.cursorPosition = final;

		// std::cout << "xPos: " << final.x << ", " << "yPos: " << final.y << std::endl;
	}

	// Reset Game
	void resetGame() {
		playerInput.startGame = false;
		playerInput.resetFlag = true;
		playerInput.isMovingBackward = false;
		playerInput.isMovingForward = false;
		playerInput.cursorPosition = glm::vec2(0.0f, 1.0f);
	}

	// Retrieve player input
	PlayerInput getPlayerInput() {
		PlayerInput returnInput = playerInput;

		playerInput.isMovingForward = false;
		playerInput.isMovingBackward = false;
		playerInput.resetFlag = false;

		return returnInput;
	}
	
private:
	glm::ivec2 screenDim;
	PlayerInput playerInput;
	ShaderProgram& shader;
};

// END EXAMPLES

int main() {
	Log::debug("Starting main");

	std::srand(static_cast<unsigned int>(std::time(0)));

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

	std::vector<GameObject> diamonds;
	GameObject diamond1("textures/diamond.png", GL_NEAREST, DEFAULT_DIAMOND_WIDTH, DEFAULT_DIAMOND_HEIGHT);
	GameObject diamond2("textures/diamond.png", GL_NEAREST, DEFAULT_DIAMOND_WIDTH, DEFAULT_DIAMOND_HEIGHT);
	GameObject diamond3("textures/diamond.png", GL_NEAREST, DEFAULT_DIAMOND_WIDTH, DEFAULT_DIAMOND_HEIGHT);
	GameObject diamond4("textures/diamond.png", GL_NEAREST, DEFAULT_DIAMOND_WIDTH, DEFAULT_DIAMOND_HEIGHT);
	diamond1.initializeDiamond();
	diamond2.initializeDiamond();
	diamond3.initializeDiamond();
	diamond4.initializeDiamond();

	// Game Score
	int score = 0;

	// RENDER LOOP
	while (!window.shouldClose()) {
		
		glfwPollEvents();

		PlayerInput input = callback->getPlayerInput();

		// Reset Score and diamonds
		if (input.resetFlag) {
			score = 0;
			diamond1.initializeDiamond();
			diamond2.initializeDiamond();
			diamond3.initializeDiamond();
			diamond4.initializeDiamond();
		}

		shader.use();

		// Clear screen
		glEnable(GL_FRAMEBUFFER_SRGB);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		/*---------------------------------------------------------------*/

		glm::mat4 diamondTransformationMatrix;

		// Update diamond1 position
		if (diamond1.appear) {
			if (isCaught(ship.position, diamond1.position) && input.startGame) {
				score++;
				ship.resizeShip(score);
				diamond1.appear = false;
			}
			else {
				diamond1.updateDiamond(input);
				diamondTransformationMatrix = diamond1.getTransformationMatrix();
				glUniformMatrix4fv(
					glGetUniformLocation(shader.getProgram(), "transformationMatrix"),
					1, GL_FALSE, glm::value_ptr(diamondTransformationMatrix)
				);
				diamond1.ggeom.bind();
				diamond1.texture.bind();
				glDrawArrays(GL_TRIANGLES, 0, 6);
				diamond1.texture.unbind();
			}
		}

		// Update diamond2 position
		if (diamond2.appear) {
			if (isCaught(ship.position, diamond2.position) && input.startGame) {
				score++;
				ship.resizeShip(score);
				diamond2.appear = false;
			}
			else {
				diamond2.updateDiamond(input);
				diamondTransformationMatrix = diamond2.getTransformationMatrix();
				glUniformMatrix4fv(
					glGetUniformLocation(shader.getProgram(), "transformationMatrix"),
					1, GL_FALSE, glm::value_ptr(diamondTransformationMatrix)
				);
				diamond2.ggeom.bind();
				diamond2.texture.bind();
				glDrawArrays(GL_TRIANGLES, 0, 6);
				diamond2.texture.unbind();
			}
		}

		// Update diamond3 position
		if (diamond3.appear) {
			if (isCaught(ship.position, diamond3.position) && input.startGame) {
				score++;
				ship.resizeShip(score);
				diamond3.appear = false;
			}
			else {
				diamond3.updateDiamond(input);
				diamondTransformationMatrix = diamond3.getTransformationMatrix();
				glUniformMatrix4fv(
					glGetUniformLocation(shader.getProgram(), "transformationMatrix"),
					1, GL_FALSE, glm::value_ptr(diamondTransformationMatrix)
				);
				diamond3.ggeom.bind();
				diamond3.texture.bind();
				glDrawArrays(GL_TRIANGLES, 0, 6);
				diamond3.texture.unbind();
			}
		}

		// Update diamond4 position
		if (diamond4.appear) {
			if (isCaught(ship.position, diamond4.position) && input.startGame) {
				score++;
				ship.resizeShip(score);
				diamond4.appear = false;
			}
			else {
				diamond4.updateDiamond(input);
				diamondTransformationMatrix = diamond4.getTransformationMatrix();
				glUniformMatrix4fv(
					glGetUniformLocation(shader.getProgram(), "transformationMatrix"),
					1, GL_FALSE, glm::value_ptr(diamondTransformationMatrix)
				);
				diamond4.ggeom.bind();
				diamond4.texture.bind();
				glDrawArrays(GL_TRIANGLES, 0, 6);
				diamond4.texture.unbind();
			}
		}
		/*---------------------------------------------------------------*/


		// Update ship position
		ship.updateShip(input);
		glm::mat4 transformationMatrix = ship.getTransformationMatrix();
		glUniformMatrix4fv(
			glGetUniformLocation(shader.getProgram(), "transformationMatrix"),
			1, GL_FALSE, glm::value_ptr(transformationMatrix)
		);

		// Render Ship
		ship.ggeom.bind();
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

		if (score == 4) {
			ImGui::Text("Winner Winner Chicken Dinner | Press [R] to reset the game");
		}
		else {
			ImGui::Text("Score: %d", score); // Second parameter gets passed into "%d"
		}

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
