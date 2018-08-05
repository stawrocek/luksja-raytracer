#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normals;
layout(location = 2) in vec2 uv;

uniform mat4 mx;

out vec2 uvCoords;

void main(){
	gl_Position = mx*vec4(position, 1.0);
	uvCoords=vec2(uv.x, uv.y);
}
