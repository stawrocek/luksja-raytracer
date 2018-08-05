# Luksja Raytracer

Very simple raytracer with OpenGL 3 interactive preview made as university project

![cat.png](./results/cat.png)
![spheres2.png](./results/spheres2.png)
![chest2.png](./results/chest2.png)
Object view:
![object_view.png](./readme_imgs/objectview.png)
# Movement

  - [r] render
  - [tab] switch between object and render mode
  - [w,s,a,d] move camera in object mode
  - [right mouse button] move camera around, change objects properties
  - [mouse wheel] zoom

# Dependencies

  - gldinding https://github.com/cginternals/glbinding#compile-instructions (checkout to `96dbfd46d67960d2e52aa9d472da99aab9f6f5a3`)
  - sdl2
  - assimp
  - nlohmann/json
  - imgui
  - glm
  - stb image

# Others
This renderer allows to change objects and camera parameters during rendering, which makes it cool tool for creating intentionaly glitched images.
![glitch1.png](./results/glitch1.png)
![glitch2.png](./results/glitch2.png)

# Afterwords

Raytracers are awesome.
Even raytracer as simple as this one can render objects of beauty:

![klocuch.png](./results/klocuch.png)
