#include "raycast.hpp"

float Raycast::cross_2d(const glm::vec3& a, const glm::vec3& b) {
	return a.x * b.y - a.y * b.x;
}

bool Raycast::raycast_line(const Ray& ray, const Line& line) {
	float bias = 0.000001f;
	glm::vec3 a = ray.origin - line.start;
	glm::vec3 b = line.end - line.start;
	glm::vec3 c(-ray.direction.y, ray.direction.x, 0.0f);

	float dot = glm::dot(b, c);
	if (std::fabs(dot) < bias) { return 0.0f; }

	float t1 = cross_2d(b, a) / dot;
	float t2 = glm::dot(a, c) / dot;
	
	if (t1 >= 0.0f && (t2 >= 0.0f && t2 <= 1.0f)) {
		return t1;
	}

	return 0.0f;
}
