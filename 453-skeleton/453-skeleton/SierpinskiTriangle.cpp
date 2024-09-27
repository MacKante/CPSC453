#include "SierpinskiTriangle.h"

// Constructors
SierpinskiTriangle::SierpinskiTriangle() : depth(0) {}
SierpinskiTriangle::SierpinskiTriangle(int depth) : depth(depth) {}

// Setters and Getters
void SierpinskiTriangle::setDepth(int newDepth) {
	this->depth = newDepth;
}

int SierpinskiTriangle::getDepth() const {
	return depth;
}

const CPU_Geometry& SierpinskiTriangle::getCPUGeometry() const {
	return cpuGeom;
}

// Sierpinski Generation Methods
void SierpinskiTriangle::draw_sierpinski_triangle() {

	glm::vec3 v0(-0.5f, -0.5f, 0.f);
	glm::vec3 v1(0.5f, -0.5f, 0.f);
	glm::vec3 v2(0.f, 0.5f, 0.f);

	generate_sierpinski_vertices(v0, v1, v2, this->depth);
	generate_sierpinski_colors(depth);
}

void SierpinskiTriangle::generate_sierpinski_vertices(glm::vec3 v0, glm::vec3 v1, glm::vec3 v2, int depth) {
	if (depth > 0) {
		// Sub-Triangles
		glm::vec3 v0v1 = (v0 + v1) / 2.0f;
		glm::vec3 v1v2 = (v1 + v2) / 2.0f;
		glm::vec3 v2v0 = (v2 + v0) / 2.0f;

		// Recursive calls
		generate_sierpinski_vertices(v0, v0v1, v2v0, depth - 1);		// Sub-Trianle 0
		generate_sierpinski_vertices(v0v1, v1, v1v2, depth - 1);		// Sub-Triangle 1
		generate_sierpinski_vertices(v2v0, v1v2, v2, depth - 1);		// Sub-Triangle 2
	} else {
		this->cpuGeom.verts.push_back(v0); // p0
		this->cpuGeom.verts.push_back(v1); // p1
		this->cpuGeom.verts.push_back(v2); // p2
	}
}

void SierpinskiTriangle::generate_sierpinski_colors(int depth) {
	cpuGeom.cols.push_back(glm::vec3(1.f, 0.f, 0.f));  // Red for v1
	cpuGeom.cols.push_back(glm::vec3(1.f, 0.f, 0.f));  // Red for v2
	cpuGeom.cols.push_back(glm::vec3(1.f, 0.f, 0.f));  // Red for v3

	cpuGeom.cols.push_back(glm::vec3(0.f, 1.f, 0.f));  // Green for v4
	cpuGeom.cols.push_back(glm::vec3(0.f, 1.f, 0.f));  // Green for v5
	cpuGeom.cols.push_back(glm::vec3(0.f, 1.f, 0.f));  // Green for v6

	cpuGeom.cols.push_back(glm::vec3(0.f, 0.f, 1.f));  // Blue for v7
	cpuGeom.cols.push_back(glm::vec3(0.f, 0.f, 1.f));  // Blue for v8
	cpuGeom.cols.push_back(glm::vec3(0.f, 0.f, 1.f));  // Blue for v9
}
