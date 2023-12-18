#version 330 core

uniform sampler2D terrainTexture;

in vec2 tex;
in vec3 fragNormal;

out vec4 FragColor;

void main() {
    FragColor = vec4(texture(terrainTexture, tex.xy).rgb, 1.0f);
    //FragColor = vec4(fragNormal.xyz, 1.0);
    //FragColor = vec4(fragHeight, fragHeight, fragHeight, 1.0f);
    //FragColor = vec4(findNormal(), 1.0f);
}