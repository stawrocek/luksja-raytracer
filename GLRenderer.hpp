#ifndef GLRENDERER_HPP
#define GLRENDERER_HPP

#include "ShaderProgram.hpp"

#include <glbinding/gl/gl.h>
#include <glbinding/Binding.h>

#include <memory>

class Camera;
class Mesh;
class Object;
class Scene;
class Sphere;
class SolidSphereData;
class Triangle;

class GLRenderer{
public:
	GLRenderer(Scene&);
	void draw(Scene&, Camera&);
	void drawSphere(Object&, Camera&);
	void drawTriangle(Object&, Camera&);
	void drawMesh(Mesh&, Camera&);
	void generateNormSphereOpenGLBuffers();
	void generateTriangleOpenGLBuffers(Triangle&);
	void setScene(Scene&);

	unsigned int vaoSphere, vboSphere, eboSphere;
	unsigned int sphereIndicesCount;
	ShaderProgram shrColor;
	ShaderProgram shrMesh;
};

#endif //GLRENDERER_HPP
