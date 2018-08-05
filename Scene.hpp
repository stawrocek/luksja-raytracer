#ifndef SCENE_HPP
#define SCENE_HPP

#include <vector>
#include <memory>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

class Object;

class Scene{
public:
	Scene(const char* filename);
	void loadFromFile(const char* filename);
	std::vector<std::shared_ptr<Object> > objects;
	std::vector<std::shared_ptr<Object> > lights;	
	struct Camera{
    glm::vec3 viewPoint;
    glm::vec3 forward;
    glm::vec3 up;
		float fov;
  } camera;
  struct Output{
    glm::ivec2 size;
    unsigned int ss=1; //super-sampling
  } output;
	unsigned int recursion=1;	
};

#endif //SCENE_HPP
