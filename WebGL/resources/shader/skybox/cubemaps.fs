#version 300 es

in highp vec3 TexCoords;

uniform samplerCube samplerTexture;

out lowp vec4 FragColor;

void main()
{
    FragColor = texture(samplerTexture, TexCoords);
}