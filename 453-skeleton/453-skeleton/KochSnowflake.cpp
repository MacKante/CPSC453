#include "KochSnowflake.h"

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



