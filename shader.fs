#version 330 core
in vec3 ucolor;
out vec4 FragColor;

void main() {
	FragColor = vec4(ucolor, 1.0f);
};
