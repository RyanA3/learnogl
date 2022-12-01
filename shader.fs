#version 330 core
out vec4 FragColor;

in vec3 aColor;
in vec2 aTexCoord;

uniform sampler2D texture0;
uniform sampler2D texture1;

void main() {
	//FragColor = mix(texture(texture0, aTexCoord), texture(texture1, aTexCoord), 1.0f);
	FragColor = texture(texture1, aTexCoord);
};
