#version 330 core
layout (location = 0) in float height;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform int resolution;
uniform float ds;

void main() {
	float x = gl_VertexID / resolution * ds;
	float z = gl_VertexID % resolution * ds;
	gl_Position = projection * view * model * vec4(x, height, z, 1.0);
}