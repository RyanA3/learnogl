#version 330 core
layout (location = 0) in float height;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform int resolution;
uniform float ds;
uniform float dss;

void main() {
	gl_Position = projection * view * model * vec4(gl_VertexID * dss, height, gl_VertexID % resolution * ds, 1.0);
}