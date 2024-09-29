#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "Geometry.h"

class SierpinskiTriangle {
	private:
		CPU_Geometry cpuGeom;	// CPU
		int depth = 0;

	public:

		// Constructor
		SierpinskiTriangle();
		SierpinskiTriangle(int depth);

		// Draw the Sierpinski Triangle
		void draw_sierpinski_triangle();

		// Making hyrule triangles
		void generate_sierpinski_vertices(glm::vec3 v0, glm::vec3 v1, glm::vec3 v2, int depth);
		void generate_sierpinski_colors(int depth);

		// Depth Methods
		void setDepth(int newDepth);
		int getDepth() const;
		int getTriangles() const;

		// CPU_GEOM
		const CPU_Geometry& getCPUGeometry() const;
		void resetCPUGeometry(int newDepth);
		void resetCPUGeometry();
};
