#include "Camera.hpp"
#include "GLRenderer.hpp"
#include "Object.hpp"
#include "RayRenderer.hpp"
#include "Scene.hpp"
#include "ShaderProgram.hpp"

#include <glbinding/gl/gl.h>
#include <glbinding/Binding.h>
#include <imgui/imgui.h>
#include "imgui_impl_sdl_gl3.h"
#include <SDL2/SDL.h>

#include <cstdio>
#include <fstream>
#include <functional>
#include <iostream>
#include <mutex>
#include <stdexcept>
#include <string>
#include <thread>
#include <unordered_map>

using namespace gl;

int winWidth=700, winHeight=700;
SDL_Window *window;

SDL_GLContext context;

void CheckSDLError(int line);
bool createWindow();
void loadScene(std::string, Scene&, Camera&, GLRenderer&);

//tab to change											
//0 -> Scene View
//1 -> Raytracer rendering View

/*enum ViewMode
{
	Scene = 0,
	Render = 1
};*/

enum ViewMode
{
    SCENE,
    RENDER
};

ViewMode viewMode=SCENE;

std::mutex pMutex;

int main(int argc, char *argv[]){
	if(argc != 2){
		std::cout << "Usage: ./luksja scene.json\n";
		return -1;
	}

	if (!createWindow())
		return -1;
	Scene scene(argv[1]);
	glEnable(GL_DEPTH_TEST);

	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui_ImplSdlGL3_Init(window);
	ImGui::StyleColorsDark();
	Camera cam(scene.camera.viewPoint, scene.camera.up
		/*, scene.camera.front*/, scene.camera.fov);
	cam.setPerspectiveMatrix(winWidth, winHeight);

	GLRenderer glRenderer(scene);
	RayRenderer rayRenderer;
	
	double now  = SDL_GetPerformanceCounter();
	double last = 0.0;
	std::unordered_map<SDL_Keycode, bool> mKeys;
	bool loop = true;
	while (loop){
		last = now;
		now = SDL_GetPerformanceCounter();
		float dt = (float)((now - last)
			 / SDL_GetPerformanceFrequency());
		SDL_Event event;

		while (SDL_PollEvent(&event)){
			ImGui_ImplSdlGL3_ProcessEvent(&event);
			if (event.type == SDL_QUIT)
				loop = false;

			else if (event.type == SDL_KEYDOWN){
				if(event.key.keysym.sym == SDLK_r){
					if(viewMode == RENDER){
						std::cout << "I am already rendering\n";
					}
					else{
						viewMode = RENDER;
						auto renderFunc = 
							[&rayRenderer, &scene, &cam](std::mutex& m) -> void{
								rayRenderer.draw(scene, cam, m);
							};
						std::thread rayRenderThread(renderFunc, std::ref(pMutex));
						rayRenderThread.detach();
					}
				}
				if(event.key.keysym.sym == SDLK_TAB){
					if(viewMode == RENDER)
						viewMode = SCENE;
					else 
						viewMode = RENDER;
				}
				mKeys[event.key.keysym.sym] = true;		
			}
			else if(event.type == SDL_KEYUP){
				if(mKeys.find(event.key.keysym.sym) != mKeys.end())
					mKeys[event.key.keysym.sym]=false;
			}
			else if(event.type == SDL_MOUSEMOTION && 
				(event.motion.state & SDL_BUTTON_RMASK)){
				cam.processMouseMovement(event.motion.xrel, -event.motion.yrel);
			}
			else if(event.type == SDL_MOUSEWHEEL){
				cam.processMouseScroll(event.wheel.y);
				cam.setPerspectiveMatrix(winWidth, winHeight);
			}
			else if(event.type == SDL_WINDOWEVENT){
				if(event.window.event == SDL_WINDOWEVENT_RESIZED){
					SDL_GetWindowSize(window, &winWidth, &winHeight);
					glViewport(0, 0, winWidth, winHeight);
					cam.setPerspectiveMatrix(winWidth, winHeight);
				}
			}
		}
		pMutex.lock();
		if(rayRenderer.previewDirty){
			rayRenderer.updateTexture();
			rayRenderer.previewDirty=false;
		}
		else if(rayRenderer.renderDone){
			rayRenderer.updateTexture();
			rayRenderer.renderDone = false;	
		}
		pMutex.unlock();
		
		if(mKeys[SDLK_a])
			cam.processKeyboard(LEFT, dt);
		if(mKeys[SDLK_d])
			cam.processKeyboard(RIGHT, dt);
		if(mKeys[SDLK_w])
			cam.processKeyboard(FORWARD, dt);
		if(mKeys[SDLK_s])
			cam.processKeyboard(BACKWARD, dt);
		
		ImGui_ImplSdlGL3_NewFrame(window);
		if(viewMode == SCENE){
			ImGui::Begin("Camera:");
				glm::vec3 pos = cam.position;
				glm::vec3 dir = cam.front;
				glm::vec3 up = cam.up;
				if(ImGui::InputFloat3("pos", glm::value_ptr(pos)))
					cam.position = pos;
				if(ImGui::InputFloat3("front", glm::value_ptr(dir)))
					cam.front = dir;
				if(ImGui::InputFloat3("right", glm::value_ptr(cam.right)))
					std::cout << "not implemented yet\n";
				if(ImGui::InputFloat3("up", glm::value_ptr(up)))
					cam.up = up;
				ImGui::InputFloat("fov", &cam.fov);
			ImGui::End();

			auto funcGui = [&](std::shared_ptr<Object> o){
				if(ImGui::Button(o->name.c_str())){
						o->guiVisible = !o->guiVisible;
					}
					if(o->guiVisible){
						ImGui::Begin(o->name.c_str());
							o->renderGui();
						ImGui::End();
					}
			};
			ImGui::Begin("Objects:");
				for(const auto& o : scene.objects){
					funcGui(o);
				}
				for(const auto& o: scene.lights){
					funcGui(o);
				}
			ImGui::End();
		}
		else{
				ImGui::Begin("Render view");
				static float imgScaleX=1.0, imgScaleY=1.0;
				ImGui::SliderFloat("Image Scale X",
					&imgScaleX, 0.0f, 1.0f, "ratio = %.3f");
				ImGui::SliderFloat("Image Scale Y",
					&imgScaleY, 0.0f, 1.0f, "ratio = %.3f");	
				ImGui::Image((void*)((int*)rayRenderer.render.id),
					ImVec2(rayRenderer.imageWidth*imgScaleX,
									rayRenderer.imageHeight*imgScaleY),
					ImVec2(0,0), ImVec2(1,1), ImColor(255,255,255,255),
					ImColor(255,255,255,128));	
				ImGui::End();
		}
		ImGui::SetNextWindowPos(ImVec2(0, winHeight-70));
		ImGui::SetNextWindowSize(ImVec2(winWidth, 70));
		ImGui::Begin("scene", 0, ImGuiWindowFlags_NoTitleBar);
			const char* items[] = {"3D", "Image"};
			static const char* item_current = items[viewMode];
			item_current = items[viewMode];
			if(ImGui::BeginCombo("View", item_current, 0)){
				for (int n = 0; n < IM_ARRAYSIZE(items); n++){
					bool is_selected = (item_current == items[n]);
					if(ImGui::Selectable(items[n], is_selected)){
						item_current = items[n];
						if(n == 0)
							viewMode = SCENE;
						else if(n == 1)
							viewMode = RENDER;
					}
					if(is_selected)
						ImGui::SetItemDefaultFocus();
				}
				ImGui::EndCombo();
			}
			if(viewMode == RENDER){
				if(rayRenderer.progress < 0.99){
					ImGui::Text("progress: %d%%",
						int(round(100.0*rayRenderer.progress)));
				}
				else{
					ImGui::Text("Done!");
				}
			}
		ImGui::End();
		
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		if(viewMode == SCENE){
			glRenderer.draw(scene, cam);
		}
		ImGui::Render();
		ImGui_ImplSdlGL3_RenderDrawData(ImGui::GetDrawData());
		SDL_GL_SwapWindow(window);
	}
	ImGui_ImplSdlGL3_Shutdown();
	SDL_GL_DeleteContext(context);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}

void loadScene(std::string fpath, Scene& scn, Camera& cam,
	GLRenderer& glRenderer){
	//scn = Scene(fpath.c_str());
	scn.loadFromFile(fpath.c_str());
	cam = Camera(scn.camera.viewPoint, scn.camera.up,
		scn.camera.fov);
	cam.setPerspectiveMatrix(winWidth, winHeight);
	glRenderer.setScene(scn);
}

bool createWindow(){
	if (SDL_Init(SDL_INIT_VIDEO) < 0){
		std::cout << "Failed to init SDL\n";
	return false;
	}
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
                      SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);  
  SDL_GL_SetSwapInterval(1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16); 
	
	window = SDL_CreateWindow("luksja raytracer",
	SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
	winWidth, winHeight, SDL_WINDOW_OPENGL);

	if (!window){
		std::cout << "Unable to create window\nkruci!!!\n";
		CheckSDLError(__LINE__);
		return false;
	}

	context = SDL_GL_CreateContext(window);
	
	glbinding::Binding::initialize();
	return true;
}

void CheckSDLError(int line = -1){
	std::string error = SDL_GetError();
	if (error != ""){
		std::cout << "SDL Error : " << error << std::endl;
		if (line != -1)
			std::cout << "\nLine : " << line << std::endl;
		SDL_ClearError();
	}
}

