#ifndef SHADER_HPP
#define SHADER_HPP

#include <glbinding/gl/gl.h>
#include <glbinding/Binding.h>

#include <string>
#include <vector>

using namespace gl;

class Shader{
public:
	Shader();
	Shader(std::string fpath, GLenum type);
	virtual ~Shader();
	void loadFromFile(std::string fpath, GLenum type);
	void loadFromString(std::string str, GLenum type);
	bool compile(GLenum type);
	GLuint getId();
	std::string getShaderName();
protected:
	std::string shaderCode;
	GLuint id;
	GLenum shaderType;
};
#endif // SHADER_HPP
