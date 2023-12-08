#version 330 core

uniform sampler2D terrainTexture;

in vec2 tex; 

out vec4 FragColor;

void main() {
    FragColor = vec4(texture(terrainTexture, tex.xy).rgb, 1.0f);
}