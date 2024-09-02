#include "transform.hpp"

std::ostream& operator<<(std::ostream& ostream, const Transform& transform) {
	return ostream << 
		"Position\n" << 
			"X: " << transform.position.x << 
			" Y: " << transform.position.y << 
			" Z: " << transform.position.z << "\n"
		"Euler Angles\n" << 
			"X: " << transform.euler_angles.x << 
			" Y: " << transform.euler_angles.y << 
			" Z: " << transform.euler_angles.z << "\n"
		"Scale\n" << 
			"X: " << transform.scale.x << 
			" Y: " << transform.scale.y << 
			" Z: " << transform.scale.z << "\n";
}