#ifndef FPS_ENGINE_RAY_HPP
#define FPS_ENGINE_RAY_HPP

#include <glm/geometric.hpp>
#include <glm/vec3.hpp>

#include "color.hpp"

const float RAY_MAX_DISTANCE = 100.0f;

struct Ray {
    glm::vec3 origin;
    glm::vec3 direction;
    float distance;
    Color color;

    Ray() : 
        origin(glm::vec3(0.0f)), 
        direction(glm::vec3(0.0f)), 
        color(Color::White), 
        distance(RAY_MAX_DISTANCE) {}
    Ray(const glm::vec3& origin, const glm::vec3& direction, const Color& color) : 
        origin(origin), 
        direction(glm::normalize(direction)), 
        color(color), 
        distance(RAY_MAX_DISTANCE) {}
    Ray(const glm::vec3& origin, 
        const glm::vec3& direction, 
        const Color& color, 
        float distance) :
        origin(origin),
        direction(glm::normalize(direction)),
        color(color),
        distance(distance) {}
    Ray(const Ray&) = default;
    Ray(Ray&&) = default;
    virtual ~Ray() = default;
};

#endif