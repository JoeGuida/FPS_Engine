#ifndef FPS_ENGINE_DRAWABLE_HPP
#define FPS_ENGINE_DRAWABLE_HPP

#include "material.hpp"

struct Drawable {
	virtual Material get_material() const = 0;
};

#endif