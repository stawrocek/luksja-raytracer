#include "PointLight.hpp"

unsigned int PointLight::pointLightCounter = 0;

PointLight::PointLight(const json& j)
:Sphere(j)
{
	//maybe it isn't nest classes hierarchy...
	if(name.empty() || name.substr(0, 6) == "SPHERE")
		name = "POINT_LIGHT"+std::to_string(++pointLightCounter);
	//...but it works
}

void PointLight::renderGui(){
	Sphere::renderGui();
	ImGui::InputFloat("power", &power);
}
