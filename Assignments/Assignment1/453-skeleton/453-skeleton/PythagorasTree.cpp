#include "PythagorasTree.h"
#include <cmath>
#include <math.h>

#define PI_4     0.785398163397448309616  // pi/4

// Constructors
PythagorasTree::PythagorasTree() : depth(0) {}
PythagorasTree::PythagorasTree(int depth) : depth(depth) {}

// Setters and Getters
void PythagorasTree::setDepth(int newDepth) {
	this->depth = newDepth;
}

int PythagorasTree::getDepth() const {
	return this->depth;
}

const CPU_Geometry& PythagorasTree::getCPUGeometry() const {
	return this->cpuGeom;
}

void PythagorasTree::resetCPUGeometry(int newDepth) {
	cpuGeom.verts.clear();
	cpuGeom.cols.clear();
	this->depth = newDepth;
}

void PythagorasTree::resetCPUGeometry() {
	cpuGeom.verts.clear();
	cpuGeom.cols.clear();
	this->depth = 0;
}

void PythagorasTree::draw_pythagoras_tree() {
	cpuGeom.verts.clear();
	cpuGeom.cols.clear();

	glm::vec3 v0(-0.125f, -0.5f, 0.0f);

	generate_pythagoras_vertices(v0, 0.25f, 0.f, this->depth);
	generate_pythagoras_colors(this->depth);
}

void PythagorasTree::generate_pythagoras_vertices(glm::vec3 v0, float sideLength, float angle, int depth) {

	// Point Calculations
	glm::vec3 p0 = v0;	// Bottom left, initial point for both triangles
	glm::vec3 p1 = p0 + glm::vec3(sideLength * cos(angle), sideLength * sin(angle), 0.f);  // Bottom right
	glm::vec3 p2 = p1 + glm::vec3(-sideLength * sin(angle), sideLength * cos(angle), 0.f); // Top right
	glm::vec3 p3 = p0 + glm::vec3(-sideLength * sin(angle), sideLength * cos(angle), 0.f); // Top left

	// First triangle (Bottom left, Bottom right, Top right)
	this->cpuGeom.verts.push_back(p0);
	this->cpuGeom.verts.push_back(p1);
	this->cpuGeom.verts.push_back(p2);

	// Second triangle (Bottom left, Top right, Top left)
	this->cpuGeom.verts.push_back(p0);
	this->cpuGeom.verts.push_back(p2);
	this->cpuGeom.verts.push_back(p3);

	if (depth > 0) {
		// Side Length = Hypotenuse / sqrt(2)
		float newSideLength = sideLength / sqrt(2.f);

		// Generate Left Smaller Square
		// Starting point is top left of base square
		// adding Pi/4 is equivalent to 45 degree rotation counter-clockwise
		float leftAngle = angle + PI_4;	
		glm::vec3 leftStartingPoint = p3;
		generate_pythagoras_vertices(leftStartingPoint, newSideLength, leftAngle, depth - 1);

		// Generate Right Smaller Square
		// Starting point is the point where to smaller squares touch, at the top of the isoceles triangle
		// subtracting Pi/4 is equivalent to 45 degree rotation clockwise
		float rightAngle = angle - PI_4;	
		glm::vec3 rightStartingPoint = p2 - glm::vec3(newSideLength * std::cos(rightAngle), newSideLength * std::sin(rightAngle), 0.f);
		generate_pythagoras_vertices(rightStartingPoint, newSideLength, rightAngle, depth - 1);
	}	
}

void PythagorasTree::generate_pythagoras_colors(int depth) {
	// Base Tree colors
	this->cpuGeom.cols.push_back(glm::vec3(0.4f, 0.2f, 0.1f));
	this->cpuGeom.cols.push_back(glm::vec3(0.4f, 0.2f, 0.1f));
	this->cpuGeom.cols.push_back(glm::vec3(0.4f, 0.2f, 0.1f));
	this->cpuGeom.cols.push_back(glm::vec3(0.4f, 0.2f, 0.1f));
	this->cpuGeom.cols.push_back(glm::vec3(0.4f, 0.2f, 0.1f));
	this->cpuGeom.cols.push_back(glm::vec3(0.4f, 0.2f, 0.1f));

	// leaft color
	for (int i = 0; i < pow(3, depth); i++) {
		this->cpuGeom.cols.push_back(glm::vec3(1.0f, 0.843f, 0.0f));
		this->cpuGeom.cols.push_back(glm::vec3(1.0f, 0.843f, 0.0f));
		this->cpuGeom.cols.push_back(glm::vec3(1.0f, 0.843f, 0.0f));
		this->cpuGeom.cols.push_back(glm::vec3(1.0f, 0.843f, 0.0f));
		this->cpuGeom.cols.push_back(glm::vec3(1.0f, 0.843f, 0.0f));
		this->cpuGeom.cols.push_back(glm::vec3(1.0f, 0.843f, 0.0f));
	}
}
