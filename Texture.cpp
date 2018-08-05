#include "Texture.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <glbinding/gl/gl.h>
#include <glbinding/Binding.h>

#include <iostream>

using namespace gl;

Texture::Texture(){

}

Texture::Texture(std::string fpath){
	if (fpath.find("assets") == std::string::npos){
		fpath = "assets/"+fpath;
	}
	std::cout << "* loading texture from " << fpath << "\n";
	unsigned char* image = stbi_load(fpath.c_str(), &width, &height, &components, STBI_rgb_alpha);
	if(image == nullptr)
		throw(std::string("Failed to load texture"));
	buildPixelsFromData(image);
	createOpenGLBuffers(image);
	stbi_image_free(image);
}

Texture::Texture(std::vector<unsigned char>& memory,
	int _width, int _height, int _components)
:width(_width), height(_height), components(_components)
{

	unsigned char* image = memory.data();
	buildPixelsFromData(image);	
	createOpenGLBuffers(image);
}

void Texture::createOpenGLBuffers(unsigned char* image){
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	if(components == 3){
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
			width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	}
	else if(components == 4){
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
			width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	}
	glBindTexture(GL_TEXTURE_2D, 0);	
}

void Texture::buildPixelsFromData(unsigned char* data){
	pixels.clear();
	pixels.resize(height);
	int linearCtr=0;
	for(int y = 0; y < height; y++){
		pixels[y].resize(width);
		for(int x = 0; x < width; x++){
			//linearCtr++;
			//std::cout << (int)data[linearCtr++] << "\n";
			//pixels[y][x].x = 0.0;data[linearCtr++];
			//pixels[y][x].y = 1.0;data[linearCtr++];
			//pixels[y][x].z = 0.0;//data[linearCtr++];
			pixels[y][x].r = data[linearCtr++]/255.0;
			pixels[y][x].g = data[linearCtr++]/255.0;
			pixels[y][x].b = data[linearCtr++]/255.0;
			linearCtr++;
		}
	}
}
