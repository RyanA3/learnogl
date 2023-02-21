#version 330 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat3 normal_matrix;

out vec3 frag_normal;
out vec3 frag_pos;

void main() {

	gl_Position = projection * view * model * vec4(pos, 1.0);

	frag_pos = vec3(model * vec4(pos, 1.0));
	frag_normal = normal_matrix * normal;

}