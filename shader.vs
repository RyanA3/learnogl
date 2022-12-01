#version 330 core
layout (location = 0) in vec3 inPos;
layout (location = 1) in vec3 inColor;
layout (location = 2) in vec2 inTexCoord;

out vec3 aColor;
out vec2 aTexCoord;

uniform mat4 transform;

void main() {
	gl_Position = transform * vec4(inPos, 1.0f);
	aColor = inColor;
	aTexCoord = inTexCoord;
}