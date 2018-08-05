#ifndef MESH_HPP
#define MESH_HPP

#include "Object.hpp"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <string>

#include "Texture.hpp"

class MeshData;

class Mesh: public Object{
public:
	Mesh(const json&);
	~Mesh();
	virtual bool intersect(const Ray& ray, float& t,
		glm::vec3& vNorm, glm::vec2& uv, glm::vec3& hitPoint) override;
	virtual glm::vec3 getColorFromUV(glm::vec2) override;
	void generateOpenGLBuffers();
	void processNode(aiNode *node, const aiScene *scene);
	void processMesh(aiMesh *mesh, const aiScene *scene);
	virtual void updateWorldSpaceVariables() override;
	std::vector<MeshData> vecMeshes;
	std::string fpath;
	
	std::string pathDiffuse="NONE";
	Texture textureDiffuse;

protected:
	static unsigned int counter;	
};

class MeshData{
public:
	struct Vertex {
		glm::vec3 pos;
		glm::vec3 normal;
		glm::vec2 uv;
	
		glm::vec3 worldPos;
	};
	struct Texture {
		unsigned int id;
		std::string type;
	};
	struct Face{
		Vertex vertices[3];
	};

	MeshData(aiMesh *mesh, const aiScene *scene);
	void generateOpenGLBuffers();
	std::vector<Face> faces;

	unsigned int vao, vbo, ebo;
};

#endif //MESH_HPP
