#version 330 core

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

struct Material
{
    sampler2D texture_diffuse1;
    sampler2D texture_reflection1;
};

uniform samplerCube samplerTexture;
uniform vec3 cameraPosition;
uniform Material material;
uniform int mapping;

out vec4 FragColor;

void main()
{
    float ratio = 1.00 / 1.52;
    vec3 viewDir = FragPos - cameraPosition;
    vec3 reflectDir = reflect(viewDir, Normal);
    vec3 refractDir = refract(viewDir, Normal, ratio);
    vec4 reflectTexture = texture(samplerTexture, reflectDir);
    vec4 reflectionTexture = texture(material.texture_reflection1, TexCoords);
    vec4 diffuseTexture = texture(material.texture_diffuse1, TexCoords);

    if(mapping == 0)
    {
        FragColor = reflectTexture;
    }
    else if(mapping == 1)
    {
        FragColor = texture(samplerTexture, refractDir);
    }
    else if(mapping == 2)
    {
        FragColor = vec4((reflectionTexture * reflectTexture + diffuseTexture).xyz, 1.0f);
    }
}