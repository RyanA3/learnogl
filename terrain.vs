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

//uniform sampler2D heightmap;

out vec2 tex;
out vec3 fragNormal;

void main() {

	int row = gl_VertexID / resolution;
	int col = gl_VertexID % resolution;

	tex = vec2(row, col) / (resolution - 1.0f);

	//float map_height = texture(heightmap, tex).r;

	gl_Position = projection * view * model * vec4(row * width * invResolution, height, col * ds, 1.0);

}