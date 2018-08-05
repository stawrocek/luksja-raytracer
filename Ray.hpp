#ifndef RAY_HPP
#define RAY_HPP

#include <glm/vec3.hpp>

class Ray{
public:
	Ray(const glm::vec3& _o, const glm::vec3& _d);
	glm::vec3 o, d;         //origin, vector
};

#endif //RAY_HPP
