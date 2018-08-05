#version 330 core

out vec4 color;

in vec2 uvCoords;
uniform vec4 obj_color;
uniform sampler2D tex0;

void main(){
	color = obj_color*texture(tex0, uvCoords);
}
