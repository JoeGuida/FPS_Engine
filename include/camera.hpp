#ifndef FPS_ENGINE_CAMERA_HPP
#define FPS_ENGINE_CAMERA_HPP

#include <glm/geometric.hpp>
#include <glm/vec3.hpp>

#include "transform.hpp"

class Camera {
private:
	glm::vec3 up;
	glm::vec3 right;

public:
	Camera(const glm::vec3& front, const glm::vec3& world_up, const Transform& transform) : 
		front(glm::normalize(front)),
		right(glm::cross(front, world_up)),
		up(glm::cross(glm::cross(front, world_up), front)),
		transform(transform) {}
		
	Camera(const Camera&) = default;
	Camera(Camera&&) = default;
	virtual ~Camera() = default;

	Camera& operator=(const Camera& camera) {
		transform.position = camera.transform.position;
		transform.euler_angles = camera.transform.euler_angles;
		transform.scale = camera.transform.scale;
		return *this;
	}

	glm::vec3 front;
	Transform transform;
};

#endif