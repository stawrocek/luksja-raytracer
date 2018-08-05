#include "Scene.hpp"

#include "Mesh.hpp"
#include "Object.hpp"
#include "PointLight.hpp"
#include "Sphere.hpp"
#include "Triangle.hpp"

// https://github.com/nlohmann/json
#include "nlohmann/json.hpp"

#include <fstream>
#include <sstream>

using json = nlohmann::json;

Scene::Scene(const char* filename){
	loadFromFile(filename);
}

void Scene::loadFromFile(const char* filename){
	std::ifstream fileStream(filename);
	std::string jsonString;
	std::string line;
	while(std::getline(fileStream,line,'\n')){
		unsigned int firstChar=0;
		for(unsigned int i = 0; i < line.size(); i++){
			if(line[i] == ' ' || line[i] == '\t')
				firstChar++;
			else
				break;
		}
		line = line.substr(firstChar, line.size()-firstChar);
		if(line[0] != '#')
			jsonString += line+"\n";
	}
	json scn = json::parse(jsonString);

	//will throw an exception if json doesn't contain 
	//camera or output
	auto const& cam = scn["camera"];
	auto const& out = scn["output"];
	Object::parseVar(cam, "ViewPoint", camera.viewPoint);
	Object::parseVar(cam, "Forward", camera.forward);
	Object::parseVar(cam, "Up", camera.up);
	Object::parseVar(cam, "fov", camera.fov);
	Object::parseVar(out, "size", output.size);
	Object::parseVar(out, "ss", output.ss);
	recursion = scn["recursion"];

	objects.clear();
	lights.clear();
	for(const auto& obj : scn["scene"]){
		if(obj["type"] == "SPHERE")
			objects.push_back(std::make_shared<Sphere>(obj));
		if(obj["type"] == "TRIANGLE")
			objects.push_back(std::make_shared<Triangle>(obj));
		if(obj["type"] == "MESH")
			objects.push_back(std::make_shared<Mesh>(obj));
		if(obj["type"] == "POINT_LIGHT")
			lights.push_back(std::make_shared<PointLight>(obj));
	}
}
