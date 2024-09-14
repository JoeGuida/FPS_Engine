#ifndef FPS_ENGINE_MINIMAP_HPP
#define FPS_ENGINE_MINIMAP_HPP

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "color.hpp"
#include "line.hpp"
#include "map.hpp"
#include "rectangle.hpp"

constexpr float MINIMAP_SCREEN_PERCENTAGE = 0.33f;

struct MiniMap {
	float width;
	float height;
	glm::vec4 bounds;
	std::vector<Color> colormap;
	Map map;

	MiniMap(const Map& map, glm::vec4 bounds, double aspect_ratio, const std::vector<Color>& colormap) :
		map(map),
		width((bounds.w - bounds.z) * MINIMAP_SCREEN_PERCENTAGE / aspect_ratio * 2.0f),
		height((bounds.w - bounds.z) * MINIMAP_SCREEN_PERCENTAGE),
		bounds(bounds),
		colormap(colormap) {}
	MiniMap(const MiniMap&) = default;
	MiniMap(MiniMap&&) = default;
	virtual ~MiniMap() = default;

	std::vector<Line> get_lines();
	std::vector<Rectangle> get_rectangles();
	std::vector<Rectangle> optimize_rectangles();
};

#endif