#include "Mesh.hpp"

#include <glbinding/gl/gl.h>
#include <glbinding/Binding.h>

#include "Ray.hpp"

using namespace gl;
unsigned int Mesh::counter = 0;

Mesh::Mesh(const json& j)
:Object(j)
{
	if(!parseVar(j, "path", fpath))
		throw std::invalid_argument("no property 'path' in MESH");	
	std::cout << "* reading mesh " << fpath << "\n";
	if(name.empty())
		name = "MESH_"+std::to_string(++counter);
	parseVar(j, "pathDiffuse", pathDiffuse);
	Assimp::Importer importer;
	const aiScene *scene = importer.ReadFile(fpath,
		aiProcess_Triangulate | aiProcess_FlipUVs);
	if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE 
		|| !scene->mRootNode) {
		throw std::invalid_argument("ERROR::ASSIMP" 
				  + std::string(importer.GetErrorString()));
	}
	processNode(scene->mRootNode, scene);
	if(pathDiffuse == "NONE"){
		throw std::invalid_argument("can't find "
			"diffuse map (.mtl or 'pathDiffuse' in .json) in "
			+ fpath);
	}
	textureDiffuse = Texture(pathDiffuse);
	hasTexture=true;
	updateWorldSpaceVariables();
}

Mesh::~Mesh(){
	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);
}

void Mesh::processNode(aiNode *node, const aiScene *scene){
	for(unsigned int i = 0; i < node->mNumMeshes; i++){
		aiMesh *mesh = scene->mMeshes[node->mMeshes[i]]; 
		processMesh(mesh, scene);			
	}
	for(unsigned int i = 0; i < node->mNumChildren; i++){
		processNode(node->mChildren[i], scene);
	}
}  

void Mesh::processMesh(aiMesh *mesh, const aiScene *scene){
	vecMeshes.push_back(MeshData(mesh, scene));
	aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
	aiTextureType type = aiTextureType_DIFFUSE;
	if(material->GetTextureCount(type) > 0){
		aiString ai_str;
		material->GetTexture(type, 0, &ai_str);
		pathDiffuse = std::string(ai_str.C_Str());
	}
}

void Mesh::generateOpenGLBuffers(){
	for(MeshData& m: vecMeshes)
		m.generateOpenGLBuffers();
}

bool Mesh::intersect(const Ray& ray, float &t,
	glm::vec3& vNorm, glm::vec2& uv, glm::vec3& hitPoint){

	constexpr float kEpsilon = 1e-8;
	float bestRes = 1000000.f;
	bool foundResult=false;
	for(const MeshData& md: vecMeshes){	
		for(const MeshData::Face tri: md.faces){
			const auto& a = tri.vertices[0];
			const auto& b = tri.vertices[1];
			const auto& c = tri.vertices[2];

			float u, v;
			glm::vec3 v0v1 = b.worldPos - a.worldPos;
			glm::vec3 v0v2 = c.worldPos - a.worldPos;
			glm::vec3 pvec = glm::cross(ray.d, v0v2);
			float det = glm::dot(v0v1,pvec);
			if (fabs(det) < kEpsilon)
				continue;
			float invDet = 1.f / det;

			glm::vec3 tvec = ray.o - a.worldPos;
			u = glm::dot(tvec, pvec) * invDet;
			if (u < 0 || u > 1)
				continue;

			glm::vec3 qvec = glm::cross(tvec, v0v1);
			v = glm::dot(ray.d,qvec) * invDet;
			if (v < 0 || u + v > 1)
				continue;

			float res = glm::dot(v0v2,qvec) * invDet;
			if(res < bestRes){
				foundResult=true;
				bestRes = res;
				t = res;
				hitPoint = ray.o + t*ray.d;
				vNorm = glm::normalize(glm::cross(v0v1, v0v2));
				const glm::vec2 &st0 = a.uv; 
				const glm::vec2 &st1 = b.uv; 
				const glm::vec2 &st2 = c.uv; 
        uv = (1.f-u-v)*st0 + u*st1 + v*st2;
				//uv = glm::vec2(u, v);
			}
		}
	}
	return foundResult;
}

void Mesh::updateWorldSpaceVariables(){
	for(auto& md: vecMeshes){
		for(auto& f: md.faces){
			for(auto& v: f.vertices){
				auto v4 = getGlobalTransformMatrix() *
					glm::vec4(v.pos.x, v.pos.y, v.pos.z, 1.f);
				v.worldPos = glm::vec3(v4.x, v4.y, v4.z);
			}
		}
	}
}

glm::vec3 Mesh::getColorFromUV(glm::vec2 uv){
	if(pathDiffuse == "NONE")
		return glm::vec3(0.f, 0.f, 0.f);
	const int x = textureDiffuse.width*uv.x;
	const int y = textureDiffuse.height*uv.y;
	//std::cout << x << " " << y << "\n";
	const auto& p = textureDiffuse.pixels[y][x];
	return glm::vec3(p.x, p.y, p.z);
	//return glm::vec3(uv.x, uv.y, 1-uv.x-uv.y);
}

//MeshData

MeshData::MeshData(aiMesh *mesh, const aiScene *scene){
	for(unsigned int i = 0; i < mesh->mNumFaces; i++){
		Face face;
		auto const &f = mesh->mFaces[i];
		for(unsigned int k = 0; k < 3; k++){
			Vertex vertex;

			auto const &v = mesh->mVertices[f.mIndices[k]];
			vertex.pos.x = v.x;
			vertex.pos.y = v.y;
			vertex.pos.z = v.z;
			
			auto const &n = mesh->mNormals[f.mIndices[k]];
			vertex.normal.x = n.x;
			vertex.normal.y = n.y;
			vertex.normal.z = n.z;
			
			if(mesh->HasTextureCoords(0)) {
				auto const &uv = mesh->mTextureCoords[0][f.mIndices[k]];
				vertex.uv.x = uv.x;
				vertex.uv.y = uv.y;
			}
			
			face.vertices[k] = vertex;	
		}
		faces.push_back(face);
	}
	/*for(unsigned int i = 0; i < faces.size(); i++){
		for(unsigned int k = 0; k < 3; k++){
			const auto& t = faces[i].vertices[k].pos;
			printf("(%lf, %lf, %lf) ", t.x, t.y, t.z);
		}
		printf("\n");
	}*/
}

void MeshData::generateOpenGLBuffers(){
	std::vector<float> vboData;
	for(unsigned int i = 0; i < faces.size(); i++){
		for(unsigned int k = 0; k < 3; k++){
			glm::vec3 pos    = faces[i].vertices[k].pos;
			glm::vec3 normal = faces[i].vertices[k].normal;
			glm::vec2 uv     = faces[i].vertices[k].uv;
			vboData.push_back(pos.x);
			vboData.push_back(pos.y);
			vboData.push_back(pos.z);
			vboData.push_back(normal.x);
			vboData.push_back(normal.y);
			vboData.push_back(normal.z);
			vboData.push_back(uv.x);
			vboData.push_back(uv.y);
		}
	}
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 
		vboData.size()*sizeof(GLfloat),
		vboData.data(), GL_STATIC_DRAW);

	GLuint stride = 8;
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
		stride * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,
		stride * sizeof(GLfloat),
		(void*)(sizeof(GLfloat)*(3)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE,
		stride * sizeof(GLfloat),
		(GLvoid*)(sizeof(GLfloat)*(6)));
	glEnableVertexAttribArray(2);
}

