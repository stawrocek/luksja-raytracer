#include "RayRenderer.hpp"

#include "Camera.hpp"
#include "Object.hpp"
#include "PointLight.hpp"
#include "Ray.hpp"
#include "Scene.hpp"

#include <fstream>
#include <iostream>
#include <vector>

#include "glm/gtx/norm.hpp"
#include "glm/glm.hpp"

glm::vec3 RayRenderer::trace(const Ray& ray, 
	const Scene& scn, unsigned int depth){
	
	std::shared_ptr<Object> closestObj = nullptr;
	float closestDist = 2000001.0;
	glm::vec3 normal, hit;
	glm::vec2 uv;
	glm::vec3 surfaceColor = glm::vec3(0.f,0.f,0.f);
	for(auto x: scn.objects){
		float fDist = 2000001.0;
		glm::vec3 n, h;
		glm::vec2 tmpUV;
		if(x->intersect(ray, fDist, n, tmpUV, h)){
			if(fDist < closestDist){
				closestDist = fDist;
				closestObj = x;
				normal = n;
				hit = h;
				uv = tmpUV;
			}
		}
	}

	if(closestObj == nullptr)
		return surfaceColor;
	glm::vec3 objColor = glm::vec3(closestObj->color.r,closestObj->color.g,
														closestObj->color.b);
	if(closestObj->hasTexture){
		objColor = closestObj->getColorFromUV(uv);
	}
	if(depth < scn.recursion){
		glm::vec3 reflectDir = glm::normalize(glm::reflect(-ray.d, normal));
		Ray r(hit + reflectDir*0.0001f, reflectDir);
		surfaceColor = 0.9f*trace(r, scn, depth+1);	
	}
	else{
		for(const auto& light: scn.lights){
			std::shared_ptr<PointLight> l =
				std::dynamic_pointer_cast<PointLight>(light);
			Ray shadowRay(hit,glm::normalize(l->getPosition()-hit));
			float transmission=1.0;
			for(const auto& x: scn.objects){
				glm::vec3 n, h;
				float fDist=100000.0;
				glm::vec2 tmpUV;
				if(x!=closestObj && x->intersect(shadowRay, fDist, n, tmpUV, h)){
					if(//fabs(fDist-closestDist) > 1.0 && 
						fDist*fDist < glm::length2(l->getPosition()-hit)){
						transmission=0.0;
						break;
					}
				}
			}
			float diff = std::max(glm::dot(normal, glm::normalize(
				hit-l->getPosition())), 0.f);
			glm::vec3 lc = glm::vec3(l->color.r, l->color.g, l->color.b);
			surfaceColor += transmission * objColor * lc * diff;
		}
	}
	if(depth == 0)
		surfaceColor += objColor;
	return surfaceColor;
}

void RayRenderer::draw(Scene& scn, Camera& cam, std::mutex& m){
	m.lock();
	progress = 0.0;
	renderDone=false;
	imageWidth = scn.output.size.x;
	imageHeight = scn.output.size.y;
	m.unlock();

	std::vector<std::vector<glm::vec3>> colors(scn.output.size.y);
	std::ofstream fOut("out.ppm");
	for(auto& v: colors)
		v.resize(scn.output.size.x);
	m.lock();
	imageData.resize(RENDERER_COLOR_COMPONENTS
		*scn.output.size.x*scn.output.size.y);	
	m.unlock();
	int ctrPreview=0;
	for(int y = colors.size()-1; y >= 0; y--){
		for(unsigned int x = 0; x < colors[y].size(); x++){
			unsigned int progY = colors.size()-1 - y;
			m.lock();
			progress = (float)(progY*scn.output.size.y+x)/
				(scn.output.size.x*scn.output.size.y);
			m.unlock();
			float imageAspectRatio = (float)scn.output.size.x / 
				scn.output.size.y;
			float fovRatio = tan((3.1415/180.0)*(cam.fov*cam.zoom/2.f));
			float px = (2*((x+0.5) / scn.output.size.x)-1)
				* imageAspectRatio * fovRatio;
			float py = (2*((y+0.5) / scn.output.size.y)-1) * fovRatio;
				
			glm::vec3 rayOrigin = cam.position;
			glm::vec3 rayDir = cam.right*px + cam.up*py + cam.front;
			rayDir = glm::normalize(rayDir);
			Ray primaryRay(rayOrigin, rayDir);
			colors[y][x] = trace(primaryRay, scn, 0);
			colors[y][x] = glm::clamp(colors[y][x], 0.f, 1.f)*255.f;
			imageData[ctrPreview++] = colors[y][x].r;
			imageData[ctrPreview++] = colors[y][x].g;
			imageData[ctrPreview++] = colors[y][x].b;
			imageData[ctrPreview++] = 255;		
		}
		m.lock();
		previewDirty=true;
		m.unlock();
	}
	m.lock();
	previewDirty=false;
	m.unlock();
/*
	for(int y = colors.size()-1; y >= 0; y--){
		for(unsigned int x = 0; x < colors[y].size(); x++){
			colors[y][x] = glm::clamp(colors[y][x], 0.f, 1.f)*255.f;
		}
	}
*/
/*
	m.lock();
	int ctr=0;
	for(int i = colors.size()-1; i >= 0; i--){
		for(unsigned int j = 0; j < colors[i].size(); j++){
			imageData[ctr++] = colors[i][j].r;
			imageData[ctr++] = colors[i][j].g;
			imageData[ctr++] = colors[i][j].b;
			imageData[ctr++] = 255; //GL_RGBA
		}
	}
	m.unlock();
*/

	fOut << "P3\n" << scn.output.size.x << " ";
	fOut << scn.output.size.y << " " << "255\n";
	for(int y = colors.size()-1; y >= 0; y--){
    for(unsigned int x = 0; x < colors[y].size(); x++){
      fOut  << (int)colors[y][x].r << " "
            << (int)colors[y][x].g << " "
            << (int)colors[y][x].b << " ";
		}
  }

	std::cout << "rendering done\n";
	m.lock();
	renderDone=true;
	m.unlock();
}

void RayRenderer::updateTexture(){
	if((int)render.id != -1)
		glDeleteTextures(1, &render.id);
	render = Texture(imageData, imageWidth, imageHeight);
}
