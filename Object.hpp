#ifndef OBJECT_HPP
#define OBJECT_HPP

#include "Transform.hpp"

#include <imgui/imgui.h>
// https://github.com/nlohmann/json
#include "nlohmann/json.hpp"

#include <string>

class Ray;

using json = nlohmann::json;

namespace glm{
	void from_json(const json& j, glm::ivec2& p);
	void from_json(const json& j, glm::vec3& p);
}

class Object: public Transform{
public:
	enum Type{
		SPHERE,
		TRIANGLE,
		POINT_LIGHT,
		MESH
	};
	Object(const json&);
	virtual ~Object();
	virtual glm::vec3 getColorFromUV(glm::vec2 uv);
	virtual void updateWorldSpaceVariables();
	virtual bool intersect(const Ray& ray, float& t,
		glm::vec3& vNorm, glm::vec2& uv, glm::vec3& hitPoint) = 0;
	virtual void renderGui();
	template <typename T>
	static bool parseVar(const json& j, std::string id, T& var){
		if(j.find(id) != j.end()){
			var = j[id].get<T>();
			return true;
		}
		return false;
	}	

	Type type;
	glm::vec4 color;
	std::string name;
	unsigned int vao=0;
	unsigned int vbo=0;
	bool guiVisible=false;
	bool hasTexture=false;
};

#endif //OBJECT_HPP
