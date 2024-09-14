#ifndef FPS_ENGINE_LINE_HPP
#define FPS_ENGINE_LINE_HPP

#include <glm/vec3.hpp>

#include "material.hpp"
#include "transform.hpp"

struct Line {
	glm::vec3 start;
	glm::vec3 end;

	Line(const glm::vec3& start, const glm::vec3& end) : start(start), end(end) {}
	Line(const Line&) = default;
	Line(Line&&) = default;
	virtual ~Line() = default;
};

#endif