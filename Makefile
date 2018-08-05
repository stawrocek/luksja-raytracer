CC=g++ 
CFLAGS=-Wall -std=c++17 -pthread -g#-fopenmp
INCLUDE=-I"./lib_include" -I"./lib_include/imgui"
LIBS="-lGL" "-lglbinding" "-lSDL2" "-lassimp"

default: luksja

luksja: main.o Object.o Transform.o ShaderProgram.o Shader.o GLRenderer.o Camera.o Scene.o Ray.o RayRenderer.o imgui.o imgui_draw.o imgui_impl_sdl_gl3.o Triangle.o Texture.o Mesh.o Sphere.o PointLight.o
	$(CC) $(CFLAGS) $(INCLUDE) main.o Transform.o Object.o ShaderProgram.o Shader.o GLRenderer.o Camera.o Ray.o RayRenderer.o Scene.o imgui.o imgui_draw.o imgui_impl_sdl_gl3.o Triangle.o Texture.o Mesh.o Sphere.o PointLight.o $(LIBS) -o luksja

main.o: main.cpp
	$(CC) $(CFLAGS) $(INCLUDE) main.cpp -c -o main.o

Transform.o: Transform.cpp
	$(CC) $(CFLAGS) $(INCLUDE) Transform.cpp -c -o Transform.o

Object.o: Object.cpp Transform.cpp
	$(CC) $(CFLAGS) $(INCLUDE) Object.cpp -c -o Object.o

ShaderProgram.o: ShaderProgram.cpp Shader.cpp
	$(CC) $(CFLAGS) $(INCLUDE) ShaderProgram.cpp -c -o ShaderProgram.o

Shader.o: Shader.cpp
	$(CC) $(CFLAGS) $(INCLUDE) Shader.cpp -c -o Shader.o

Triangle.o: Triangle.cpp
	$(CC) $(CFLAGS) $(INCLUDE) Triangle.cpp -c -o Triangle.o

GLRenderer.o: GLRenderer.cpp
	$(CC) $(CFLAGS) $(INCLUDE) GLRenderer.cpp -c -o GLRenderer.o

Camera.o: Camera.cpp Transform.cpp
	$(CC) $(CFLAGS) $(INCLUDE) Camera.cpp -c -o Camera.o

Ray.o: Ray.cpp
	$(CC) $(CFLAGS) $(INCLUDE) Ray.cpp -c -o Ray.o

Scene.o: Scene.cpp Object.cpp
	$(CC) $(CFLAGS) $(INCLUDE) Scene.cpp -c -o Scene.o

RayRenderer.o: RayRenderer.cpp
	$(CC) $(CFLAGS) $(INCLUDE) RayRenderer.cpp -c -o RayRenderer.o

Texture.o: Texture.cpp
	$(CC) $(CFLAGS) $(INCLUDE) Texture.cpp -c -o Texture.o

Mesh.o: Mesh.cpp
	$(CC) $(CFLAGS) $(INCLUDE) Mesh.cpp -c -o Mesh.o

PointLight.o: PointLight.cpp
	$(CC) $(CFLAGS) $(INCLUDE) PointLight.cpp -c -o PointLight.o

Sphere.o: Sphere.cpp
	$(CC) $(CFLAGS) $(INCLUDE) Sphere.cpp -c -o Sphere.o

imgui.o: lib_include/imgui/imgui.cpp
	$(CC) $(CFLAGS) $(INCLUDE) lib_include/imgui/imgui.cpp -c -o imgui.o

imgui_draw.o: lib_include/imgui/imgui_draw.cpp
	$(CC) $(CFLAGS) $(INCLUDE) lib_include/imgui/imgui_draw.cpp -c -o imgui_draw.o

imgui_impl_sdl_gl3.o: imgui_impl_sdl_gl3.cpp
	$(CC) $(CFLAGS) $(INCLUDE) imgui_impl_sdl_gl3.cpp -c -o imgui_impl_sdl_gl3.o

clean:
	rm -f *.o luksja out.ppm
