#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "Geometry.h"

class PythagorasTree {
private:
	CPU_Geometry cpuGeom;	// CPU
	int depth = 0;

public:

	// Constructor
	PythagorasTree();
	PythagorasTree(int depth);

	// Draw the Sierpinski Triangle
	void draw_pythagoras_tree();

	// Making squares
	void generate_pythagoras_vertices(glm::vec3 v0, float sideLength, float angle, int depth);
	void generate_pythagoras_colors(int depth);

	// Depth Methods
	void setDepth(int newDepth);
	int getDepth() const;

	// CPU_GEOM
	const CPU_Geometry& getCPUGeometry() const;
	void resetCPUGeometry(int newDepth);
	void resetCPUGeometry();
};
