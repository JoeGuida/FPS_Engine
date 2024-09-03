#ifndef FPS_ENGINE_CAMERA_HPP
#define FPS_ENGINE_CAMERA_HPP

#include <glm/geometric.hpp>
#include <glm/vec3.hpp>

#include "transform.hpp"

class Camera {
public:
    Camera(const glm::vec3& front, 
           const glm::vec3& world_up, 
           float fov_radians, 
           const Transform& transform) : 
        front(glm::normalize(front)),
        right(glm::cross(front, world_up)),
        up(glm::cross(glm::cross(front, world_up), front)),
        fov_radians(fov_radians),
        transform(transform) {}
        
    Camera(const Camera&) = default;
    Camera(Camera&&) = default;
    virtual ~Camera() = default;

    Camera& operator=(const Camera& camera) {
        fov_radians = camera.fov_radians;
        front = camera.front;
        right = camera.right;
        up = camera.up;
        transform.position = camera.transform.position;
        transform.euler_angles = camera.transform.euler_angles;
        transform.scale = camera.transform.scale;
        return *this;
    }

    float fov_radians;
    glm::vec3 front;
    glm::vec3 right;
    glm::vec3 up;
    Transform transform;
};

#endif