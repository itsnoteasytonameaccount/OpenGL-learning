#version 330 core

in VS_OUT {
    vec4 position;
    vec4 normal;
    vec2 texCoords;
} fs_in;

struct Material
{
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;
};

uniform Material material;

out vec4 FragColor;

void main()
{
    FragColor = texture(material.texture_diffuse1, fs_in.texCoords);
}