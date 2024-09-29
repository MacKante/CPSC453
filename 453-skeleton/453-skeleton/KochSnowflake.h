#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "Geometry.h"

class KochSnowflake {
private:
	CPU_Geometry cpuGeom;	// CPU Geometry
	int depth = 0;

public:

	// Constructor
	KochSnowflake();
	KochSnowflake(int depth);

	// Draw the Sierpinski Triangle
	void draw_koch_snowflake();

	// Making hyrule triangles
	void generate_koch_vertices(glm::vec3 v0, glm::vec3 v1, glm::vec3 v2, int depth);
	void generate_koch_colors(int depth);

	// Depth Methods
	void setDepth(int newDepth);
	int getDepth() const;
	int getLines() const;

	// CPU_GEOM
	const CPU_Geometry& getCPUGeometry() const;
	void resetCPUGeometry(int newDepth);
	void resetCPUGeometry();
};
