#ifndef TRIANGLE_HPP
#define TRIANGLE_HPP

#include "Object.hpp"

class Ray;

class Triangle: public Object{
public:
	Triangle(const json&);
	virtual void updateWorldSpaceVariables() override;
	virtual bool intersect(const Ray& ray, float& t,
		glm::vec3& vNorm, glm::vec2& uv, glm::vec3& hitPoint) override;

	glm::vec3 a, b, c;
protected:
	glm::vec3 worldA, worldB, worldC;
	static unsigned int counter;
};

#endif //TRIANGLE_HPP
