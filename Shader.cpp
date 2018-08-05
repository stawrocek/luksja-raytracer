#include "Shader.hpp"

#include <sstream>
#include <iostream>
#include <streambuf>
#include <fstream>
#include <cstdio>

Shader::Shader()
:id(-1)
{}

Shader::Shader(std::string fpath, GLenum type){
    loadFromFile(fpath, type);
}

Shader::~Shader(){
    glDeleteShader(id);
}

void Shader::loadFromFile(std::string fpath, GLenum type){
    std::ifstream file(fpath);

    if(!file){
        printf("[error]: couldn't open %s", fpath.c_str());
    }
    shaderCode = std::string((std::istreambuf_iterator<char>(file)),
    std::istreambuf_iterator<char>());
		compile(type);
}

void Shader::loadFromString(std::string str, GLenum type){
    shaderCode = str;
    compile(type);
}

bool Shader::compile(GLenum type){
    shaderType = type;
    id = glCreateShader(type);
    const GLchar* tmpCode = &shaderCode[0];
    glShaderSource(id, 1, &tmpCode, NULL);
    glCompileShader(id);
    // Check for compile time errors
    GLint success;
    GLchar infoLog[512];
    glGetShaderiv(id, GL_COMPILE_STATUS, &success);
    if (!success){
        glGetShaderInfoLog(id, 512, NULL, infoLog);
        printf("ERROR::SHADER::COMPILATION_FAILED: %s, (%s)", 
					infoLog, getShaderName().c_str());
    }
    return success;
}

GLuint Shader::getId(){
    return id;
}

std::string Shader::getShaderName(){
    if(shaderType == GL_VERTEX_SHADER)return "VERTEX_SHADER";
    if(shaderType == GL_FRAGMENT_SHADER)return "FRAGMENT_SHADER";
    //opengl ES 3.2 and higher
    #ifdef GEOMETRY_SHADER_ENABLED
    if(shaderType == GL_GEOMETRY_SHADER) return "GEOMETRY_SHADER";
    #endif
    return "[error] UNKNOWN_SHADER [error]";
}

