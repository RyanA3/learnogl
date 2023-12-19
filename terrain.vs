#version 330 core
layout (location = 0) in float height;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform int resolution;
uniform float ds; // = width / resolution

out vec2 tex;
out vec3 fragNormal;

void main() {

	int row = gl_VertexID / resolution;
	int col = gl_VertexID % resolution;

	tex = vec2(row, col);

	gl_Position = projection * view * model * vec4(row * ds, height, col * ds, 1.0);

}