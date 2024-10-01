#include "DragonCurve.h"

#include <math.h>

// Contstructors
DragonCurve::DragonCurve() : depth(0) {}
DragonCurve::DragonCurve(int depth) : depth(depth) {}

// Setters and Getters
int DragonCurve::getDepth() const {
	return this->depth;
}

void DragonCurve::setDepth(int newDepth) {
	this->depth = newDepth;
}

int DragonCurve::getLines() const {
	return static_cast<int>(this->cpuGeom.verts.size());
}

const CPU_Geometry& DragonCurve::getCPUGeometry() const {
	return cpuGeom;
}

void DragonCurve::resetCPUGeometry(int newDepth) {
	cpuGeom.verts.clear();
	cpuGeom.cols.clear();
	this->depth = newDepth;
}

void DragonCurve::resetCPUGeometry() {
	cpuGeom.verts.clear();
	cpuGeom.cols.clear();
	this->depth = 0;
}


// Generation Methods
void DragonCurve::draw_dragon_curve() {
	glm::vec3 v0(-0.5f, 0.0f, 0.0f);
	glm::vec3 v1(0.5f, 0.0f, 0.0f);

	generate_dragon_vertices(v0, v1, this->depth); // v0 -> v1
}

void DragonCurve::generate_dragon_vertices(glm::vec3 p0, glm::vec3 p1, int depth) {
	if (depth > 0) {
		// extra point calculations
		glm::vec3 middlepoint = (p0 + p1) * 0.5f;				// Middlepoint
		glm::vec3 direction = p1 - p0;							// Direction vector from p0 to p1
		float height = glm::length(direction) / 2.0f;			// Height of the triangle

		// Perpendicular vector to direction
		glm::vec3 perpendicular(direction.y, -direction.x, 0.0f);
		perpendicular = glm::normalize(perpendicular) * height;

		// Add the middlepoint and the perpendicular vector
		glm::vec3 p2 = middlepoint + perpendicular;

		// Recursive calls for each segment
		generate_dragon_vertices(p0, p2, depth - 1);
		generate_dragon_vertices(p1, p2, depth - 1);
	}
	else {
		this->cpuGeom.verts.push_back(p0);
		this->cpuGeom.verts.push_back(p1);

		// colors
		this->cpuGeom.cols.push_back(glm::vec3(1.f, 1.f, 1.f));
		this->cpuGeom.cols.push_back(glm::vec3(1.f, 1.f, 1.f));
	}
}
