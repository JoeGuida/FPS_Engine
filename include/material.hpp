#ifndef FPS_ENGINE_MATERIAL_HPP
#define FPS_ENGINE_MATERIAL_HPP

#include <glm/vec3.hpp>

struct Material {
	glm::vec3 color;

	Material(const glm::vec3& color) : color(color) {}
	Material(const Material&) = default;
	Material(Material&&) = default;
	virtual ~Material() = default;
};

#endif