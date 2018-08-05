#include "Object.hpp"

#include "Ray.hpp"

#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <stdexcept>

using json = nlohmann::json;

namespace glm{
	void from_json(const json& j, glm::ivec2& p){
		p.x = j[0].get<int>();
		p.y = j[1].get<int>();
	}
	void from_json(const json& j, glm::vec3& p){
		p.x = j[0].get<float>();
		p.y = j[1].get<float>();
		p.z = j[2].get<float>();
	}
	void from_json(const json& j, glm::vec4& p){
		p.x = j[0].get<float>();
		p.y = j[1].get<float>();
		p.z = j[2].get<float>();
		p.w = j[3].get<float>();
	}
}

Object::Object(const json& j)
:color(glm::vec4(0.0, 0.0, 0.0, 1.0))
{
	auto const& t = j["type"];
	if(t == "SPHERE")	type = SPHERE;
	else if(t == "TRIANGLE") type = TRIANGLE;
	else if(t == "POINT_LIGHT") type = POINT_LIGHT;
	else if(t == "MESH") type = MESH;
	parseVar(j, "color", color);
	glm::vec3 pos, scale, rot;
	if(parseVar(j, "pos", pos))
		setPosition(pos);
	if(parseVar(j, "scale", scale))
		setScale(scale);
	if(parseVar(j, "rot", rot))
		setRotation(rot);
	parseVar(j, "name", name);
}

Object::~Object(){

}

glm::vec3 Object::getColorFromUV(glm::vec2 uv){
	//any object with texture will overwrite it
	//default is textureless object
	return glm::vec3(1.f, 1.f, 1.f);
}

void Object::updateWorldSpaceVariables(){

}

void Object::renderGui(){
	if(ImGui::Button(name.c_str())){
		guiVisible = !guiVisible;	
	}
	//if(guiVisible){
	//	ImGui::Begin(name.c_str());
			ImGui::SliderFloat4("color", glm::value_ptr(color),
				0.0f, 1.0f);
			glm::vec3 pos = getPosition();
			glm::vec3 rot = getRotation();
			glm::vec3 scale = getScale();
			if(ImGui::InputFloat3("pos", glm::value_ptr(pos))){
				setPosition(pos);
				updateWorldSpaceVariables();
			}
			if(ImGui::InputFloat3("rot", glm::value_ptr(rot))){
				setRotation(rot);
				updateWorldSpaceVariables();
			}
			if(ImGui::InputFloat3("scale", glm::value_ptr(scale))){
				setScale(scale);
				updateWorldSpaceVariables();
			}
	//	ImGui::End();
	//}
}

