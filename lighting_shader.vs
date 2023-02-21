#version 330 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat3 normal_matrix;

out vec3 leNormal;
out vec3 leFragPos;

void main() {

	leNormal = normal_matrix * normal;
	leFragPos = vec3(model * vec4(pos, 1.0));
	gl_Position = projection * view * model * vec4(pos, 1.0);

}