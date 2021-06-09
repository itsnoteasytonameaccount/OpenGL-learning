#version 330 core

in vec3 TexCoords;

uniform samplerCube samplerTexture;

out vec4 FragColor;

void main()
{
    FragColor = texture(samplerTexture, TexCoords);
}