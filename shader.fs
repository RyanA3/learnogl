#version 330 core
out vec4 FragColor;

in vec3 aColor;
in vec2 aTexCoord;

uniform sampler2D texture0;
uniform sampler2D texture1;

uniform bool color_mode;

void main() {
	if(color_mode) {
		FragColor = vec4(aColor, 1.0f);
	} else {
		FragColor = mix(texture(texture0, aTexCoord), texture(texture1, aTexCoord), 0.6f);
	}
};
