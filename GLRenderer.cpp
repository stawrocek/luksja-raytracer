#include "GLRenderer.hpp"

#include "Camera.hpp"
#include "Mesh.hpp"
#include "Object.hpp"
#include "PointLight.hpp"
#include "Scene.hpp"
#include "Sphere.hpp"
#include "Triangle.hpp"

using namespace gl;

GLRenderer::GLRenderer(Scene& scn)
:shrColor({
	std::make_pair("shaders/simple.vert", GL_VERTEX_SHADER),
	std::make_pair("shaders/simple.frag", GL_FRAGMENT_SHADER)}),
shrMesh({
	std::make_pair("shaders/mesh.vert", GL_VERTEX_SHADER),
	std::make_pair("shaders/mesh.frag", GL_FRAGMENT_SHADER)})
{
	generateNormSphereOpenGLBuffers();

	for(auto& obj: scn.objects){
		if(obj->type == Object::TRIANGLE){
			std::shared_ptr<Triangle> t = 
				std::dynamic_pointer_cast<Triangle>(obj);
			generateTriangleOpenGLBuffers(*t);
		}
		else if(obj->type == Object::SPHERE){
			obj->vao = vaoSphere;
			obj->vbo = vboSphere;
		}
		else if(obj->type == Object::MESH){
			std::shared_ptr<Mesh> m =
				std::dynamic_pointer_cast<Mesh>(obj);
			m->generateOpenGLBuffers();
		}
	}
	for(auto& obj: scn.lights){
		if(obj->type == Object::POINT_LIGHT){
			obj->vao = vaoSphere;
			obj->vbo = vboSphere;
		}
	}
}

void GLRenderer::draw(Scene &scene, Camera& cam){
	auto funcDraw = [&](std::shared_ptr<Object> o){
		if(o->type == Object::SPHERE){
			drawSphere(*o, cam);
		}
		else if(o->type == Object::POINT_LIGHT){
			drawSphere(*o, cam);
		}
		else if(o->type == Object::TRIANGLE){
			drawTriangle(*o, cam);
		}
		else if(o->type == Object::MESH){
			std::shared_ptr<Mesh> m =
				std::dynamic_pointer_cast<Mesh>(o);
			drawMesh(*m, cam);
		}
	};
	
	for(auto obj : scene.objects){
		funcDraw(obj);
	}
	for(auto light : scene.lights){
		funcDraw(light);
	}
}

void GLRenderer::drawSphere(Object& obj, Camera& cam){
	glUseProgram(shrColor.getId());
	shrColor.setUniform("obj_color", obj.color);
	glm::mat4 proj = cam.getCameraCombinedMatrix();
	glm::mat4 model = obj.getGlobalTransformMatrix();
	glm::mat4 mx = proj * model;
	shrColor.setUniform("mx", mx);
	glBindVertexArray(obj.vao);
		//glDrawArrays(GL_TRIANGLES, 0, 3);
		glDrawElements(GL_TRIANGLES, sphereIndicesCount,
			GL_UNSIGNED_SHORT, nullptr);
	glBindVertexArray(0);
}

void GLRenderer::drawTriangle(Object& obj, Camera& cam){
	glUseProgram(shrColor.getId());
	shrColor.setUniform("obj_color", obj.color);
	glm::mat4 proj = cam.getCameraCombinedMatrix();
	glm::mat4 model = obj.getGlobalTransformMatrix();
	glm::mat4 mx = proj * model;
	shrColor.setUniform("mx", mx);
	glBindVertexArray(obj.vao);
		glDrawArrays(GL_TRIANGLES, 0, 3);
	glBindVertexArray(0);
}

void GLRenderer::drawMesh(Mesh& obj, Camera& cam){
	for(MeshData const& md: obj.vecMeshes){
		glUseProgram(shrMesh.getId());
		glBindTexture(GL_TEXTURE_2D, obj.textureDiffuse.id);
		shrMesh.setUniform("obj_color", obj.color);
		glm::mat4 proj = cam.getCameraCombinedMatrix();
		glm::mat4 model = obj.getGlobalTransformMatrix();
		glm::mat4 mx = proj * model;
		shrMesh.setUniform("mx", mx);
  	glBindVertexArray(md.vao);
			glDrawArrays(GL_TRIANGLES, 0, 3*md.faces.size());
		glBindVertexArray(0);
	}
}

void GLRenderer::generateTriangleOpenGLBuffers(Triangle& t){
	float vertices[] = {
		t.a.x, t.a.y, t.a.z,
		t.b.x, t.b.y, t.b.z,
		t.c.x, t.c.y, t.c.z
	};

	gl::glGenVertexArrays(1, &t.vao);
	gl::glGenBuffers(1, &t.vbo);
	gl::glBindVertexArray(t.vao);

	gl::glBindBuffer(gl::GL_ARRAY_BUFFER, t.vbo);
	gl::glBufferData(gl::GL_ARRAY_BUFFER, sizeof(vertices),
		vertices, gl::GL_STATIC_DRAW);
	gl::glVertexAttribPointer(0, 3, gl::GL_FLOAT, gl::GL_FALSE,
		3 * sizeof(float), (void*)0);
	gl::glEnableVertexAttribArray(0);
	gl::glBindBuffer(gl::GL_ARRAY_BUFFER, 0);

	gl::glBindVertexArray(0);
}

void GLRenderer::generateNormSphereOpenGLBuffers(){
	SolidSphereData sphere = SolidSphereData(1.0, 60, 60);
	sphereIndicesCount = sphere.indices.size();
	glGenVertexArrays(1, &vaoSphere);
	glGenBuffers(1, &vboSphere);
	glGenBuffers(1, &eboSphere);

	glBindVertexArray(vaoSphere);
	glBindBuffer(GL_ARRAY_BUFFER, vboSphere);
	glBufferData(GL_ARRAY_BUFFER,
		sphere.verticesAndNormals.size()*sizeof(GLfloat),
		sphere.verticesAndNormals.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,
		6*sizeof(GLfloat),(void*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,
		6*sizeof(GLfloat),(void*)(3*sizeof(float)));

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboSphere);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,
		sphere.indices.size()*sizeof(GLushort),
		sphere.indices.data(), GL_STATIC_DRAW);
	glBindVertexArray(0);	
}

void GLRenderer::setScene(Scene& scn){

}
