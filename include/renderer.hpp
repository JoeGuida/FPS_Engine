#ifndef FPS_ENGINE_RENDERER_HPP
#define FPS_ENGINE_RENDERER_HPP

#include <glad/glad.h>
#include <glm/vec3.hpp>

#include <array>

#include "line.hpp"
#include "ray.hpp"
#include "rectangle.hpp"
#include "shader.hpp"

class Renderer {
private:
    static uint32_t VAO;
    static uint32_t VBO;
    static uint32_t EBO;

public:
    static void initialize();
    static void draw(const Line& line, Shader& shader);
    static void draw(const Ray& ray, Shader& shader);
    static void draw(const Rectangle& rectangle, Shader& shader);
};

#endif