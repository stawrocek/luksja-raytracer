#include "ShaderProgram.hpp"

#include <algorithm>
#include <cstdio>

ShaderProgram::ShaderProgram(
	const std::initializer_list<std::pair<std::string, 
																				GLenum>> &listShaders){
	id = glCreateProgram();
	std::vector<Shader*> vecShaders;
	for (auto shaderPair : listShaders) {
		vecShaders.push_back(new Shader(shaderPair.first, 
																		shaderPair.second));
		glAttachShader(id, vecShaders.back()->getId());
	}
	glLinkProgram(id);
	GLint success;
	GLchar infoLog[512];
	glGetProgramiv(id, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(id, 512, NULL, infoLog);
		printf("ERROR::SHADER::PROGRAM::LINKING_FAILED: %s", infoLog);
	}
	//destructor of Shader will call glDeleteShader on every element of vecShader
	for (unsigned int i = 0; i < vecShaders.size(); i++) {
		delete vecShaders[i];
	}
}

ShaderProgram::ShaderProgram(
	const std::initializer_list<Shader> &listShaders){
  
	id = glCreateProgram();
	for (auto shr : listShaders) {
		glAttachShader(id, shr.getId());
	}
	glLinkProgram(id);
	GLint success;
	GLchar infoLog[512];
	glGetProgramiv(id, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(id, 512, NULL, infoLog);
		printf("ERROR::SHADER::PROGRAM::LINKING_FAILED: %s", infoLog);
	}
}

ShaderProgram::~ShaderProgram(){
	glDeleteProgram(id);
}

GLuint ShaderProgram::getId(){
	return id;
}

void ShaderProgram::run(){
	glUseProgram(id);
}

void ShaderProgram::setUniform(GLuint loc, const GLfloat &v){
	glUniform1f(loc, v);
}

void ShaderProgram::setUniform(GLuint loc, const GLuint &v){
	glUniform1i(loc, v);
}

void ShaderProgram::setUniform(GLuint loc, const GLint &v){
	glUniform1i(loc, v);
}

void ShaderProgram::setUniform(GLuint loc, const glm::vec3 &v){
	glUniform3f(loc, v.x, v.y, v.z);
}

void ShaderProgram::setUniform(GLuint loc, const glm::vec4 &v){
	glUniform4f(loc, v.x, v.y, v.z, v.w);
}

void ShaderProgram::setUniform(GLint loc, const glm::mat2 &mx) {
	glUniformMatrix2fv(loc, 1, GL_FALSE, glm::value_ptr(mx));
}

void ShaderProgram::setUniform(GLint loc, const glm::mat3 &mx) {
	glUniformMatrix3fv(loc, 1, GL_FALSE, glm::value_ptr(mx));
}

void ShaderProgram::setUniform(GLint loc, const glm::mat4 &mx) {
	glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(mx));
}

GLint ShaderProgram::getAttribLocation(std::string& attribDescription){
	return glGetAttribLocation(id, attribDescription.c_str());
}
