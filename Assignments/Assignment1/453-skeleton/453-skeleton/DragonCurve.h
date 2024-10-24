#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "Geometry.h"

class DragonCurve {
private:
	CPU_Geometry cpuGeom;	// CPU Geometry
	int depth = 0;

public:

	// Constructor
	DragonCurve();
	DragonCurve(int depth);

	// Draw the Sierpinski Triangle
	void draw_dragon_curve();

	// Making lines
	void generate_dragon_vertices(glm::vec3 p0, glm::vec3 p1, int depth);
	// void generate_koch_colors(int depth);

	// Depth Methods
	void setDepth(int newDepth);
	int getDepth() const;
	int getLines() const;

	// CPU_GEOM
	const CPU_Geometry& getCPUGeometry() const;
	void resetCPUGeometry(int newDepth);
	void resetCPUGeometry();
};
