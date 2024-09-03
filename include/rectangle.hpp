#ifndef FPS_ENGINE_RECTANGLE_HPP
#define FPS_ENGINE_RECTANGLE_HPP

#include <glm/vec2.hpp>
#include <array>

#include "material.hpp"
#include "transform.hpp"

class Rectangle {
private:
    glm::vec2 size;
    std::array<uint32_t, 6> indices;
    std::array<float, 12> vertices;

public:
    Rectangle(float x, 
              float y, 
              const Material& material, 
              const Transform& transform) : 
        size(x, y), 
        material(material), 
        transform(transform) 
    {
        vertices = {
            -size.x / 2.0f,  size.y / 2.0f, 0.0f,
             size.x / 2.0f,  size.y / 2.0f, 0.0f,
            -size.x / 2.0f, -size.y / 2.0f, 0.0f,
             size.x / 2.0f, -size.y / 2.0f, 0.0f
        };

        indices = { 0, 1, 3, 0, 2, 3 };
    }

    Rectangle(float x, 
              const Material& material, 
              const Transform& transform) : 
        size(x, x), 
        material(material), 
        transform(transform) 
    {
        vertices = {
            -size.x / 2.0f,  size.y / 2.0f, 0.0f,
             size.x / 2.0f,  size.y / 2.0f, 0.0f,
            -size.x / 2.0f, -size.y / 2.0f, 0.0f,
             size.x / 2.0f, -size.y / 2.0f, 0.0f
        };

        indices = { 0, 1, 3, 0, 2, 3 };
    }

    Rectangle(const Rectangle&) = default;
    Rectangle(Rectangle&&) = default;
    virtual ~Rectangle() = default;

    std::array<uint32_t, 6> get_indices() const { return indices; }
    std::array<float, 12> get_vertices() const { return vertices; }

    Material material;
    Transform transform;
};

#endif