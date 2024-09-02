#ifndef FPS_ENGINE_RENDERER_HPP
#define FPS_ENGINE_RENDERER_HPP

#include <glad/glad.h>
#include <cstdint>

#include "rectangle.hpp"
#include "shader.hpp"

class Renderer {
private:
	static uint32_t VAO;
	static uint32_t VBO;
	static uint32_t EBO;

public:
	static void initialize();
	static void draw(const Rectangle& rectangle, Shader& shader);
};

#endif