#version 330 core
layout (location = 0) in float height;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform int resolution;
uniform float width;
uniform float invResolution;
uniform float invResolutionSquare;
uniform float ds;
uniform float dss;

out vec2 tex;

void main() {
	int row = gl_VertexID / resolution;
	int col = gl_VertexID % resolution;
	tex = vec2(row, col) / (resolution - 1.0f);
	gl_Position = projection * view * model * vec4(gl_VertexID * dss, height, col * ds, 1.0);

}