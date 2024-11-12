#version 460 core

layout(location = 4) in vec4 pos;

layout(std140, binding = 0) uniform Matrices{
    vec4 resolution;
};

void main(){
	gl_Position = vec4(pos.x / resolution.x * 2 - 1, pos.y / resolution.y * 2 - 1, 0.0, 1.0);
}