#ifndef RAYRENDERER_HPP
#define RAYRENDERER_HPP

#include "Texture.hpp"

#include <glm/vec3.hpp>

#include <mutex>

class Camera;
class Ray;
class Scene;

class RayRenderer{
public:
	void draw(Scene&, Camera&, std::mutex& m);
	glm::vec3 trace(const Ray&, const Scene&, unsigned int depth);
	void updateTexture();   //must be called from main thread
													//where OpenGL context was created

	float progress=0.f;
	Texture render;

	std::vector<unsigned char> imageData;
	int imageWidth, imageHeight;
	bool previewDirty=false;
	bool renderDone=false;
protected:
	const unsigned int RENDERER_COLOR_COMPONENTS = 4; //GL_RGB

};

#endif //RAY_RENDERER_HPP
