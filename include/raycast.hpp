#ifndef FPS_ENGINE_RAYCAST_HPP
#define FPS_ENGINE_RAYCAST_HPP

#include <glm/vec3.hpp>

#include "line.hpp"
#include "ray.hpp"

struct Raycast {
	static float cross_2d(const glm::vec3& a, const glm::vec3& b);
	static bool raycast_line(const Ray& ray, const Line& line);
};

#endif
