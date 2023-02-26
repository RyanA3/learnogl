#version 330 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 aTexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat3 normal_matrix;

out vec3 frag_normal;
out vec3 frag_pos;
out vec2 TexCoords;

void main() {

	frag_pos = vec3(model * vec4(pos, 1.0));
	frag_normal = normal_matrix * normal;
	TexCoords = aTexCoords;

	gl_Position = projection * view * model * vec4(pos, 1.0);

}