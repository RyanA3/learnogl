#version 330 core
layout (location = 0) in vec3 pos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform float chunk_width;
uniform int resolution;

void main() {
	//float x = ((gl_VertexID / 2) % resolution) * chunk_width / resolution;
	//float z = ((gl_VertexID % 2)) * chunk_width / resolution;
	//float x = gl_VertexID;
	//float z = gl_VertexID % 2;
	gl_Position = projection * view * model * vec4(pos, 1.0);
}