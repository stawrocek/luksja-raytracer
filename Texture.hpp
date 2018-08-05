#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <glm/glm.hpp>

#include <string>
#include <vector>

class Texture{
public:
	Texture();
	Texture(std::string fpath);
	Texture(std::vector<unsigned char>& memory,
		int _width, int _height, int _components=3);
	int width, height, components;
	unsigned int id = -1;
	std::vector<std::vector<glm::vec3>> pixels;

protected:
	void createOpenGLBuffers(unsigned char* image);
	void buildPixelsFromData(unsigned char* data);
};

#endif //TEXTURE_HPP
