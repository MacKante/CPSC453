#pragma once

#include "Geometry.h"
#include <glm/gtx/transform.hpp>

struct UnitSphere {
	CPU_Geometry m_cpu_geom;
	GPU_Geometry m_gpu_geom;
	glm::mat4 m_model;
	GLsizei m_size;
	void generateGeometry(float radius);
};
