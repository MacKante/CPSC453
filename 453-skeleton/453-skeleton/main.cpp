#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "Geometry.h"
#include "GLDebug.h"
#include "Log.h"
#include "ShaderProgram.h"
#include "Shader.h"
#include "Window.h"


// EXAMPLE CALLBACKS
class MyCallbacks : public CallbackInterface {

public:
	MyCallbacks(ShaderProgram& shader) : shader(shader) {}

	virtual void keyCallback(int key, int scancode, int action, int mods) {
		if (key == GLFW_KEY_R && action == GLFW_PRESS) {
			shader.recompile();
		}
	}

private:
	ShaderProgram& shader;
};

class MyCallbacks2 : public CallbackInterface {

public:
	MyCallbacks2() {}

	virtual void keyCallback(int key, int scancode, int action, int mods) {
		if (key == GLFW_KEY_R && action == GLFW_PRESS) {
			std::cout << "called back" << std::endl;
		}
	}
};
// END EXAMPLES

int main() {
	Log::debug("Starting main");

	// WINDOW
	glfwInit();
	Window window(800, 800, "CPSC 453"); // can set callbacks at construction if desired

	GLDebug::enable();

	// SHADERS
	ShaderProgram shader("shaders/test.vert", "shaders/test.frag");

	// CALLBACKS
	window.setCallbacks(std::make_shared<MyCallbacks>(shader)); // can also update callbacks to new ones

	// GEOMETRY
	CPU_Geometry cpuGeom;
	GPU_Geometry gpuGeom;

	// Define the vertices of the Sierpinski triangle
	cpuGeom.verts.push_back(glm::vec3(-0.5f, -0.5f, 0.f)); // v1
	cpuGeom.verts.push_back(glm::vec3(0.f, -0.5f, 0.f));   // v2
	cpuGeom.verts.push_back(glm::vec3(-0.25f, 0.f, 0.f));  // v3

	cpuGeom.verts.push_back(glm::vec3(0.f, -0.5f, 0.f));   // v4
	cpuGeom.verts.push_back(glm::vec3(0.5f, -0.5f, 0.f));  // v5
	cpuGeom.verts.push_back(glm::vec3(0.25f, 0.f, 0.f));   // v6

	cpuGeom.verts.push_back(glm::vec3(-0.25f, 0.f, 0.f));  // v7 (repeat to match triangles)
	cpuGeom.verts.push_back(glm::vec3(0.25f, 0.f, 0.f));   // v8
	cpuGeom.verts.push_back(glm::vec3(0.f, 0.5f, 0.f));    // v9

	// Define corresponding colors for each vertex
	cpuGeom.cols.push_back(glm::vec3(1.f, 0.f, 0.f));  // Red for v1
	cpuGeom.cols.push_back(glm::vec3(1.f, 0.f, 0.f));  // Red for v2
	cpuGeom.cols.push_back(glm::vec3(1.f, 0.f, 0.f));  // Red for v3

	cpuGeom.cols.push_back(glm::vec3(0.f, 1.f, 0.f));  // Green for v4
	cpuGeom.cols.push_back(glm::vec3(0.f, 1.f, 0.f));  // Green for v5
	cpuGeom.cols.push_back(glm::vec3(0.f, 1.f, 0.f));  // Green for v6

	cpuGeom.cols.push_back(glm::vec3(0.f, 0.f, 1.f));  // Blue for v7
	cpuGeom.cols.push_back(glm::vec3(0.f, 0.f, 1.f));  // Blue for v8
	cpuGeom.cols.push_back(glm::vec3(0.f, 0.f, 1.f));  // Blue for v9

	gpuGeom.setVerts(cpuGeom.verts);
	gpuGeom.setCols(cpuGeom.cols);

	// RENDER LOOP
	while (!window.shouldClose()) {
		glfwPollEvents();

		shader.use();
		gpuGeom.bind();

		glEnable(GL_FRAMEBUFFER_SRGB);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glDrawArrays(GL_TRIANGLES, 0, 9);
		glDisable(GL_FRAMEBUFFER_SRGB); // disable sRGB for things like imgui

		window.swapBuffers();
	}

	glfwTerminate();
	return 0;
}
