#version 460 core

struct VertexData{
    vec4 pos;
    vec4 col;
};

layout(std430, binding = 5) readonly buffer vertexAttr{
    VertexData vertices[];
};

layout(std140, binding = 0) uniform Matrices{
    vec4 resolution;
};

layout(location = 0) in vec2 aTexCoords;
out vec2 TexCoords;

void main(){
    TexCoords = aTexCoords;
    gl_Position = vec4(vertices[gl_VertexID].pos.x / resolution.x * 2 - 1, vertices[gl_VertexID].pos.y / resolution.y * 2 - 1, 0., 1.);
}