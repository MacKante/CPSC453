#include "KochSnowflake.h"

#include <cmath>

// Contstructors
KochSnowflake::KochSnowflake() : depth(0) {}
KochSnowflake::KochSnowflake(int depth) : depth(depth) {}

// Setters and Getters
int KochSnowflake::getDepth() const {
	return this->depth;
}

void KochSnowflake::setDepth(int newDepth) {
	this->depth = newDepth;
}

int KochSnowflake::getLines() const {
	return static_cast<int>(this->cpuGeom.verts.size());
}

const CPU_Geometry& KochSnowflake::getCPUGeometry() const {
	return cpuGeom;
}

void KochSnowflake::resetCPUGeometry(int newDepth) {
	cpuGeom.verts.clear();
	cpuGeom.cols.clear();
	this->depth = newDepth;
}

void KochSnowflake::resetCPUGeometry() {
	cpuGeom.verts.clear();
	cpuGeom.cols.clear();
	this->depth = 0;
}


// Generation Methods
void KochSnowflake::draw_koch_snowflake() {
	glm::vec3 v0(-0.5f, -0.5f, 0.f);
	glm::vec3 v1(0.5f, -0.5f, 0.f);
	glm::vec3 v2(0.f, 0.5f, 0.f);

	generate_koch_vertices(v0, v1, this->depth); // v0 -> v1
	generate_koch_vertices(v1, v2, this->depth); // v1 -> v2
	generate_koch_vertices(v2, v0, this->depth); // v2 -> v0
}

void KochSnowflake::generate_koch_vertices(glm::vec3 p0, glm::vec3 p1, int depth) {
	if (depth > 0) {
		// One third of the length
		glm::vec3 length = (p1 - p0) / 3.f;
		float magnitude = glm::length(p1 - p0);

		// extra point calculations
		glm::vec3 p2 = p0 + length;
		glm::vec3 p3 = p1 - length;

		
		glm::vec3 middleSegment = p3 - p2;
		glm::vec3 p4(
			middleSegment.x - length.y * sqrt(3.0f) / 2.0f,   // Rotate 60 degrees for x
			middleSegment.y + length.x * sqrt(3.0f) / 2.0f,   // Rotate 60 degrees for y
			0.f
		);
	
		// Recursive calls for each segment
		generate_koch_vertices(p0, p2, depth - 1); // p0 -> p2
		generate_koch_vertices(p2, p4, depth - 1); // p2 -> p4
		generate_koch_vertices(p4, p3, depth - 1); // p4 -> p3
		generate_koch_vertices(p3, p1, depth - 1); // p3 -> p1
	}
	else {
		// Vertices
		this->cpuGeom.verts.push_back(p0);
		this->cpuGeom.verts.push_back(p1);

		// colors
		this->cpuGeom.cols.push_back(glm::vec3(1.f, 1.f, 1.f));
		this->cpuGeom.cols.push_back(glm::vec3(1.f, 1.f, 1.f));
	}
}
