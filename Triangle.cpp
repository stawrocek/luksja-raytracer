#include "Triangle.hpp"

#include "Ray.hpp"

unsigned int Triangle::counter = 0;

Triangle::Triangle(const json& j)
:Object(j)
{
	if(!parseVar(j, "a", a))
		throw std::invalid_argument("no property 'a' in TRIANGLE");
	if(!parseVar(j, "b", b))
		throw std::invalid_argument("no property 'b' in TRIANGLE");
	if(!parseVar(j, "c", c))
		throw std::invalid_argument("no property 'c' in TRIANGLE");
	if(name.empty())
		name = "TRIANGLE_"+std::to_string(++counter);
	updateWorldSpaceVariables();
}

void Triangle::updateWorldSpaceVariables(){
	auto a4 = getGlobalTransformMatrix()*
		glm::vec4(a.x, a.y, a.z, 1.f);
	auto b4 = getGlobalTransformMatrix()*
		glm::vec4(b.x, b.y, b.z, 1.f);
	auto c4 = getGlobalTransformMatrix()*
		glm::vec4(c.x, c.y, c.z, 1.f);
	worldA = glm::vec3(a4.x, a4.y, a4.z);
	worldB = glm::vec3(b4.x, b4.y, b4.z);
	worldC = glm::vec3(c4.x, c4.y, c4.z);
}

bool Triangle::intersect(const Ray& ray, float &t,
  glm::vec3& vNorm, glm::vec2& uv, glm::vec3& hitPoint){
	
	constexpr float kEpsilon = 1e-8;
	float u, v;

	glm::vec3 v0v1 = worldB - worldA;
	glm::vec3 v0v2 = worldC - worldA;
	glm::vec3 pvec = glm::cross(ray.d, v0v2);
	float det = glm::dot(v0v1,pvec);
	if (fabs(det) < kEpsilon) return false;
	float invDet = 1 / det;

	glm::vec3 tvec = ray.o - worldA;
	u = glm::dot(tvec, pvec) * invDet;
	if (u < 0 || u > 1) return false;

	glm::vec3 qvec = glm::cross(tvec, v0v1);
	v = glm::dot(ray.d, qvec) * invDet;
	if (v < 0 || u + v > 1) return false;

	t = glm::dot(v0v2,qvec) * invDet;
	hitPoint = ray.o + t*ray.d;
	vNorm = glm::normalize(glm::cross(v0v1, v0v2));
	uv = glm::vec2(u, v);
	return true;
}

