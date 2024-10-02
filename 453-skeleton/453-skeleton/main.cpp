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
#include "PythagorasTree.h"
#include "KochSnowflake.h"
#include "DragonCurve.h"

// Defines for MAX
#define SIERPINSKI_MAX 7
#define PYTHAGORAS_MAX 8
#define KOCH_MAX 5
#define DRAGON_MAX 12

// Global Variables
int g_depthCount_sierpinski = 0;
int g_depthCount_pythagoras = 0;
int g_depthCount_koch = 0;
int g_depthCount_dragon = 0;

int g_fractalModeCount = 0;
GLenum g_fractalPrimitive = GL_TRIANGLES;


// EXAMPLE CALLBACKS
class MyCallbacks : public CallbackInterface {

public:
	MyCallbacks(ShaderProgram& shader) : shader(shader) {}

	virtual void keyCallback(int key, int scancode, int action, int mods) {
		if (key == GLFW_KEY_R && action == GLFW_PRESS) {
			shader.recompile();
		}
		else if (key == GLFW_KEY_LEFT && action == GLFW_PRESS) {
			g_depthCount_sierpinski--;
			g_depthCount_pythagoras--;
			g_depthCount_koch--;
			g_depthCount_dragon--;

			// Sierpinski Max
			if (g_depthCount_sierpinski < 0) {
				g_depthCount_sierpinski = SIERPINSKI_MAX;
			}

			if (g_depthCount_pythagoras < 0) {
				g_depthCount_pythagoras = PYTHAGORAS_MAX;
			}

			if (g_depthCount_koch < 0) {
				g_depthCount_koch = KOCH_MAX;
			}

			if (g_depthCount_dragon < 0) {
				g_depthCount_dragon = DRAGON_MAX;
			}
		}
		else if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS) {
			g_depthCount_sierpinski++;
			g_depthCount_pythagoras++;
			g_depthCount_koch++;
			g_depthCount_dragon++;

			// Sierpinski Max
			if (g_depthCount_sierpinski > SIERPINSKI_MAX) {
				g_depthCount_sierpinski = 0;
			}

			// Pythagoras Max
			if (g_depthCount_pythagoras > PYTHAGORAS_MAX) {
				g_depthCount_pythagoras = 0;
			}

			// Koch Max
			if (g_depthCount_koch > KOCH_MAX) {
				g_depthCount_koch = 0;
			}

			// Dragon Max
			if (g_depthCount_dragon > DRAGON_MAX) {
				g_depthCount_dragon = 0;
			}
		}
		else if (key == GLFW_KEY_DOWN && action == GLFW_PRESS) {
			g_depthCount_sierpinski = 0;
			g_depthCount_pythagoras = 0;
			g_depthCount_koch = 0;
			g_depthCount_dragon = 0;

			g_fractalModeCount++;
			if (g_fractalModeCount > 3) {
				g_fractalModeCount = 0;
			}
		}
		else if (key == GLFW_KEY_UP && action == GLFW_PRESS) {
			g_depthCount_sierpinski = 0;
			g_depthCount_pythagoras = 0;
			g_depthCount_koch = 0;
			g_depthCount_dragon = 0;

			g_fractalModeCount--;
			if (g_fractalModeCount < 0) {
				g_fractalModeCount = 3;
			}
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

	// GEOMETRY
	CPU_Geometry cpuGeom;
	GPU_Geometry gpuGeom;

	/* Sierpinski Triangle */
	SierpinskiTriangle sierpinski = SierpinskiTriangle();
	//sierpinski.draw_sierpinski_triangle();
	//cpuGeom = sierpinski.getCPUGeometry();

	/* Pythagoras Tree */
	PythagorasTree pythagoras = PythagorasTree();
	//pythagoras.draw_pythagoras_tree();
	//cpuGeom = pythagoras.getCPUGeometry();

	/* Koch Snowflake */
	KochSnowflake koch = KochSnowflake();
	//koch.draw_koch_snowflake();
	//cpuGeom = koch.getCPUGeometry();

	/* Dragon Curve */
	DragonCurve dragon = DragonCurve();
	//dragon.draw_dragon_curve();
	//cpuGeom = dragon.getCPUGeometry();

	// RENDER LOOP
	while (!window.shouldClose()) {
		glfwPollEvents();

		switch (g_fractalModeCount) {
		case 0:
			sierpinski.setDepth(g_depthCount_sierpinski);
			sierpinski.draw_sierpinski_triangle();
			cpuGeom = sierpinski.getCPUGeometry();
			g_fractalPrimitive = GL_TRIANGLES;
			break;
		case 1:
			pythagoras.setDepth(g_depthCount_pythagoras);
			pythagoras.draw_pythagoras_tree();
			cpuGeom = pythagoras.getCPUGeometry();
			g_fractalPrimitive = GL_TRIANGLES;
			break;
		case 2:
			koch.setDepth(g_depthCount_koch);
			koch.draw_koch_snowflake();
			cpuGeom = koch.getCPUGeometry();
			g_fractalPrimitive = GL_LINES;
			break;
		case 3:
			dragon.setDepth(g_depthCount_dragon);
			dragon.draw_dragon_curve();
			cpuGeom = dragon.getCPUGeometry();
			g_fractalPrimitive = GL_LINES;
			break;
		}

		// Setting Geometry
		gpuGeom.setVerts(cpuGeom.verts);
		gpuGeom.setCols(cpuGeom.cols);

		shader.use();
		gpuGeom.bind();

		glEnable(GL_FRAMEBUFFER_SRGB);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glDrawArrays(g_fractalPrimitive, 0, cpuGeom.verts.size());
		glDisable(GL_FRAMEBUFFER_SRGB); // disable sRGB for things like imgui

		window.swapBuffers();
	}

	glfwTerminate();
	return 0;
}

