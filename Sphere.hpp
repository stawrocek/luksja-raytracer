#ifndef SPHERE_HPP
#define SPHERE_HPP

#include "Object.hpp"

class Sphere: public Object{
public:
	Sphere(const json&);
	virtual bool intersect(const Ray& ray, float& t,
		glm::vec3& vNorm, glm::vec2& uv, glm::vec3& hitPoint) override;
	virtual void renderGui() override;
	float radius;
	static unsigned int counter;
};

class SolidSphereData{
public:
	std::vector<float> vertices;
	std::vector<float> normals;
	std::vector<float> texcoords;
	std::vector<float> verticesAndNormals;
	std::vector<ushort> indices;

	SolidSphereData();
	SolidSphereData(float radius, unsigned int rings,
		unsigned int sectors);
};

#endif //SPHERE_HPP
