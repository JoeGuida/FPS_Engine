#ifndef FPS_ENGINE_RAY_HPP
#define FPS_ENGINE_RAY_HPP

#include <glm/geometric.hpp>
#include <glm/vec3.hpp>

#include "color.hpp"
#include "drawable.hpp"
#include "material.hpp"

const float RAY_MAX_DISTANCE = 5.0f;

struct Ray : public Drawable {
    glm::vec3 origin;
    glm::vec3 direction;
    float distance;
    Material material;

    Ray() : 
        origin(glm::vec3(0.0f)), 
        direction(glm::vec3(0.0f)), 
        material(Material(Color::White)), 
        distance(RAY_MAX_DISTANCE) {}
    Ray(const glm::vec3& origin, const glm::vec3& direction, const Material& material) : 
        origin(origin), 
        direction(glm::normalize(direction)), 
        material(material),
        distance(RAY_MAX_DISTANCE) {}
    Ray(const glm::vec3& origin, 
        const glm::vec3& direction, 
        const Material& material, 
        float distance) :
        origin(origin),
        direction(glm::normalize(direction)),
        material(material),
        distance(distance) {}

    Ray(const Ray&) = default;
    Ray(Ray&&) = default;
    virtual ~Ray() = default;

    Material get_material() const override { return material; }
};

#endif