#include "UnitSphere.h"
#include <glm/gtx/transform.hpp>

#include "cmath"
#include "corecrt_math_defines.h"

// Defines
#define HALF_CIRCLE_CONTROL_POINTS 11
#define B_SPLINE_SUBDIVISION_ITERATIONS 3
#define SURFACE_OF_REVOLUTION_SLICES 30

// Definitions
std::vector<glm::vec3> generateHalfCircleControlPoints(float radius, int segments);
std::vector<glm::vec3> chaikinCurveSubdivision(const std::vector<glm::vec3>& coarsePoints, int iterations);
std::pair<std::vector<glm::vec3>, std::vector<glm::vec2>> surfaceOfRevolution(std::vector<glm::vec3>& curvePoints, int n_slices, float radius);
std::vector<glm::vec2> generateTextureCoordinates(const std::vector<glm::vec3>& verts, int n_slices, int n_stacks);
std::vector<glm::vec3> computeNormals(const std::vector<glm::vec3>& verts);


void UnitSphere::generateGeometry(float radius) {
	// Generate the Half circle control points
	std::vector<glm::vec3> controlPoints = generateHalfCircleControlPoints(radius, HALF_CIRCLE_CONTROL_POINTS);

	// Chaikin's subdivision for smoothing
	std::vector<glm::vec3> smoothCurve = chaikinCurveSubdivision(controlPoints, B_SPLINE_SUBDIVISION_ITERATIONS);

	// Generate sphere from smooth half circle curve
	auto [surface, texCoord] = surfaceOfRevolution(smoothCurve, SURFACE_OF_REVOLUTION_SLICES, radius);  // Generate the surface verts and texCoords

	// CPU Geometry: set the vertices and texture coordinates
	m_cpu_geom.verts = surface;        
	m_cpu_geom.cols = std::vector<glm::vec3>(m_cpu_geom.verts.size(), glm::vec3(0.f, 0.f, 0.f)); // Set default color (black)
	m_cpu_geom.texCoords = texCoord; // Set texture coordinates 

	// GPU Geometry: bind and send the data to the GPU
	m_gpu_geom.bind();
	m_gpu_geom.setVerts(m_cpu_geom.verts);
	m_gpu_geom.setCols(m_cpu_geom.cols);
	m_gpu_geom.setTexCoords(m_cpu_geom.texCoords);  // Assuming setTexCoords method exists in m_gpu_geom

	// GLSize: the number of vertices
	m_size = m_cpu_geom.verts.size();

	// Compute normals for the vertices - TODO
	// std::vector<glm::vec3> normals = computeNormals(surface);
	// m_cpu_geom.normals = std::vector<glm::vec3>(m_cpu_geom.verts.size(), glm::vec3(0.f, 0.f, 0.f));
	// m_gpu_geom.setNormals(m_cpu_geom.normals);
}

/*----------------- Chaikin Curve Subdivision Algorithm -----------------*/
std::vector<glm::vec3> chaikinCurveSubdivision(const std::vector<glm::vec3>& coarsePoints, int iterations) {
	// No iterations or not enough points to subdivide
	int n = coarsePoints.size();
	if ((iterations == 0) || (n < 2)) {
		return coarsePoints;
	}

	std::vector<glm::vec3> C = coarsePoints;
	std::vector<glm::vec3> F;

	// Special mask at the beginning
	F.push_back(C[0]);
	F.push_back(0.5f * C[0] + 0.5f * C[1]);

	// Periodic mask for the interior points
	for (int i = 1; i < n - 2; ++i) {
		F.push_back(0.75f * C[i] + 0.25f * C[i + 1]);
		F.push_back(0.25f * C[i] + 0.75f * C[i + 1]);
	}

	// Special mask at the end
	F.push_back(0.5f * C[n - 2] + 0.5f * C[n - 1]);
	F.push_back(C[n - 1]);

	// Perform subdivision on fine points if there are more iterations
	return chaikinCurveSubdivision(F, iterations - 1);
}

/*-------------------- Generate Surface of Revolution -------------------*/
std::pair<std::vector<glm::vec3>, std::vector<glm::vec2>> surfaceOfRevolution(std::vector<glm::vec3>& curvePoints, int n_slices, float radius) {
	// Vectors to hold the 3D vertices and texture coordinates
	std::vector<glm::vec3> surface_verts;
	std::vector<glm::vec2> surface_uvs;

	// Angle between each slice
	float angle_step = 2.0f * M_PI / n_slices;

	// For each curve point, generate vertices and their texture coordinates
	for (int i = 0; i < curvePoints.size() - 1; i++) {
		glm::vec3 p1 = curvePoints[i];
		glm::vec3 p2 = curvePoints[i + 1];

		for (int j = 0; j < n_slices; j++) {
			float angle = j * angle_step;
			float next_angle = (j + 1) * angle_step;

			// Generate 3D vertices
			glm::vec3 v1(p1.x * cos(angle), p1.y, p1.x * sin(angle));
			glm::vec3 v2(p2.x * cos(angle), p2.y, p2.x * sin(angle));
			glm::vec3 v3(p2.x * cos(next_angle), p2.y, p2.x * sin(next_angle));
			glm::vec3 v4(p1.x * cos(next_angle), p1.y, p1.x * sin(next_angle));

			// Add the vertices to the surface_verts vector
			surface_verts.push_back(v1);
			surface_verts.push_back(v2);
			surface_verts.push_back(v3);
			surface_verts.push_back(v1);
			surface_verts.push_back(v3);
			surface_verts.push_back(v4);

			// Calculate texture coordinates
			float u1 = angle / (2 * M_PI);   // Normalize angle to [0, 1]
			float u2 = next_angle / (2 * M_PI);

			// Assuming y_min = -radius and y_max = radius for the half-sphere
			float v1_coord = (p1.y + radius) / (2 * radius); // Normalize y to [0, 1]
			float v2_coord = (p2.y + radius) / (2 * radius);

			// Add the UV coordinates to the surface_uvs vector
			surface_uvs.push_back(glm::vec2(u1, v1_coord));
			surface_uvs.push_back(glm::vec2(u1, v2_coord));
			surface_uvs.push_back(glm::vec2(u2, v2_coord));
			surface_uvs.push_back(glm::vec2(u1, v1_coord));
			surface_uvs.push_back(glm::vec2(u2, v2_coord));
			surface_uvs.push_back(glm::vec2(u2, v1_coord));
		}
	}

	// Return both vertices and texture coordinates as a pair
	return std::make_pair(surface_verts, surface_uvs);
}


/*-------------------- Generate Sphere Control Points -------------------*/
std::vector<glm::vec3> generateHalfCircleControlPoints(float radius, int segments) {
	std::vector<glm::vec3> controlPoints;

	// Generate points for a half-circle in the X-Y plane
	for (int i = 0; i <= segments; ++i) {
		float angle = M_PI * i / segments; // Angle from -π/2 to π/2
		float x = radius * sin(angle);    // X-coordinate
		float y = radius * cos(angle);    // Y-coordinate
		controlPoints.push_back(glm::vec3(x, y, 0.0f)); // z = 0 for the Z-axis revolution
	}

	return controlPoints;
}

/*-------------------- Texture Coordinates -------------------*/
std::vector<glm::vec2> generateTextureCoordinates(const std::vector<glm::vec3>& verts, int n_slices, int n_stacks) {
	std::vector<glm::vec2> tex_coords;

	// Loop through vertices and assign u, v coordinates
	for (int stack = 0; stack <= n_stacks; ++stack) {
		float v = static_cast<float>(stack) / n_stacks; // Latitude (normalized)

		for (int slice = 0; slice < n_slices; ++slice) {
			float u = static_cast<float>(slice) / n_slices; // Longitude (normalized)
			tex_coords.push_back(glm::vec2(u, v));
		}
	}

	return tex_coords;
}


/*-------------------- Generate Sphere Control Points -------------------*/
std::vector<glm::vec3> computeNormals(const std::vector<glm::vec3>& verts) {
	std::vector<glm::vec3> normals(verts.size());

	for (size_t i = 0; i < verts.size(); i++) {
		// Normalize the vertex position to compute the normal
		glm::vec3 normal = glm::normalize(verts[i]);
		normals[i] = normal;
	}

	return normals;
}

