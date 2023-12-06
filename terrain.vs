#version 330 core
layout (location = 0) in float height;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform float chunk_width;
uniform int resolution;
uniform float ds;

void main() {
	float x = gl_VertexID / resolution * chunk_width;
	float z = mod(gl_VertexID, resolution) * chunk_width;
	gl_Position = projection * view * model * vec4(x, height, z, 1.0);
}