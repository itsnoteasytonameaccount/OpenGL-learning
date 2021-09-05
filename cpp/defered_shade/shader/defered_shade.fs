#version 330 core

layout (location = 0) out vec3 position;
layout (location = 1) out vec3 normal;
layout (location = 2) out vec4 color;

struct Material
{
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;
};

uniform Material material;

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoord;
} fs_in;

void main()
{
    normal = fs_in.Normal;
    position = fs_in.FragColor;
    color.rgb = texture(material.texture_diffuse1, fs_in.TexCoord).rgb;
    color.a = texture(material.texture_specular1, fs_in.TexCoord).r;
}