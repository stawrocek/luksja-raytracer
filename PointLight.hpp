#ifndef POINT_LIGHT_HPP
#define POINT_LIGHT_HPP

#include "Sphere.hpp"

class PointLight: public Sphere{
public:
	PointLight(const json&);
	virtual void renderGui() override;
	static unsigned int pointLightCounter;
	float power = 1.0;
};

#endif //POINT_LIGHT_HPP
