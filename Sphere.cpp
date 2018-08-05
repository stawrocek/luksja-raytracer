#include "Sphere.hpp"

#include "Ray.hpp"

unsigned int Sphere::counter = 0;

Sphere::Sphere(const json& j)
:Object(j)
{
	if(!parseVar(j, "radius", radius))
		throw std::invalid_argument("no property 'radius' in SPHERE");
	if(name.empty())
		name = "SPHERE_"+std::to_string(++counter);
	setScale(radius, radius, radius);
}

bool Sphere::intersect(const Ray& ray, float &t,
	glm::vec3& vNorm, glm::vec2& uv, glm::vec3& hitPoint){

	glm::vec3 oc = ray.o - getPosition();
	const double b = 2 * glm::dot(oc, ray.d);
	const double c = glm::dot(oc, oc) - radius*radius;
	double disc = b*b - 4 * c;
	if (disc < 1e-4) return false;
	disc = sqrt(disc);
	const double t0 = -b - disc;
	const double t1 = -b + disc;
	t = (t0 < t1) ? t0 : t1;
	t /= 2.f;
	hitPoint = ray.o+t*ray.d;
	vNorm = (getPosition()-hitPoint)/radius;
	return true;
}

void Sphere::renderGui(){
	Object::renderGui();
	if(ImGui::InputFloat("radius", &radius)){
		setScale(radius, radius, radius);
	}
}

SolidSphereData::SolidSphereData(){}
SolidSphereData::SolidSphereData(float radius,
	unsigned int rings, unsigned int sectors){
	float const R = 1./(float)(rings-1);
	float const S = 1./(float)(sectors-1);
	unsigned int r, s;

	vertices.resize(rings * sectors * 3);
	normals.resize(rings * sectors * 3);
	texcoords.resize(rings * sectors * 2);
	verticesAndNormals.resize(rings * sectors * 6);

	std::vector<float>::iterator vn = verticesAndNormals.begin();

	float m_pi_2 = 1.570796326;
	float m_pi = 3.141592653;
	for(r = 0; r < rings; r++){
		for(s = 0; s < sectors; s++){
			float const y = sin( -m_pi_2 + m_pi * r * R );
			float const x = cos(2*m_pi * s * S) * sin( m_pi * r * R );
			float const z = sin(2*m_pi * s * S) * sin( m_pi * r * R );
			
			*vn++ = x * radius;
			*vn++ = y * radius;
			*vn++ = z * radius;

			*vn++ = x;
			*vn++ = y;
			*vn++ = z;
		}
	}

	indices.resize(rings * sectors * 6);
	std::vector<ushort>::iterator i = indices.begin();
	for(r = 0; r < rings-1; r++){
		for(s = 0; s < sectors-1; s++){
			*i++ = r * sectors + s;
			*i++ = r * sectors + (s+1);
			*i++ = (r+1) * sectors + (s+1);

			*i++ = r * sectors + s;
			*i++ = (r+1) * sectors + (s+1);
			*i++ = (r+1) * sectors + s;
		}
	}
}
