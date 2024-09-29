#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "Geometry.h"
#include "GLDebug.h"
#include "Log.h"
#include "ShaderProgram.h"
#include "Shader.h"
#include "Window.h"

#include "SierpinskiTriangle.h"


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
	Window window(1000, 1000, "CPSC 453"); // can set callbacks at construction if desired

	GLDebug::enable();

	// SHADERS
	ShaderProgram shader("shaders/test.vert", "shaders/test.frag");

	// CALLBACKS
	window.setCallbacks(std::make_shared<MyCallbacks>(shader)); // can also update callbacks to new ones

	glm::vec3 v0(-0.5f, -0.5f, 0.f);	// V0
	glm::vec3 v1(0.5f, -0.5f, 0.f);		// V1
	glm::vec3 v2(0.f, 0.5f, 0.f);		// V2

	//SierpinskiTriangle sierpinski = SierpinskiTriangle(6);
	//sierpinski.draw_sierpinski_triangle();

	//// GEOMETRY
	//CPU_Geometry cpuGeom = sierpinski.getCPUGeometry();
	//GPU_Geometry gpuGeom;

	CPU_Geometry cpuGeom;
	GPU_Geometry gpuGeom;

	cpuGeom.verts.push_back(v0);
	cpuGeom.verts.push_back(v1);
	cpuGeom.verts.push_back(v1);
	cpuGeom.verts.push_back(v2);
	cpuGeom.verts.push_back(v2);
	cpuGeom.verts.push_back(v0);

	cpuGeom.cols.push_back(glm::vec3(1.f, 1.f, 1.f));
	cpuGeom.cols.push_back(glm::vec3(1.f, 1.f, 1.f));
	cpuGeom.cols.push_back(glm::vec3(1.f, 1.f, 1.f));
	cpuGeom.cols.push_back(glm::vec3(1.f, 1.f, 1.f));
	cpuGeom.cols.push_back(glm::vec3(1.f, 1.f, 1.f));
	cpuGeom.cols.push_back(glm::vec3(1.f, 1.f, 1.f));

	gpuGeom.setVerts(cpuGeom.verts);
	gpuGeom.setCols(cpuGeom.cols);

	// RENDER LOOP
	while (!window.shouldClose()) {
		glfwPollEvents();

		shader.use();
		gpuGeom.bind();

		glEnable(GL_FRAMEBUFFER_SRGB);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glDrawArrays(GL_LINES, 0, cpuGeom.verts.size());
		glDisable(GL_FRAMEBUFFER_SRGB); // disable sRGB for things like imgui

		window.swapBuffers();
	}

	glfwTerminate();
	return 0;
}

