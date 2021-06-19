#version 330 core

struct Light {
    vec3 position;

    vec3 diffuse;
    vec3 specular;
    vec3 ambient;
};

struct Material {
    float shininess;

    sampler2D tex;
    sampler2D specularTex;
};


in VS_OUT {
    vec3 fragPos;
    vec3 normal;
    vec2 texCoord;
} fs_in;

#define LIGHT_NUMBER 4

uniform Light lights[LIGHT_NUMBER];
uniform Material material;
uniform int scene;

layout (location = 0) out vec4 fragColor;
layout (location = 1) out vec4 brightColor;

vec3 compute_light(int i, vec3 specular, vec3 diffuse)
{
    vec3 lightDir = normalize(lights[i].position - fs_in.fragPos);
    vec3 normal = normalize(fs_in.normal);
    float diff = max(dot(lightDir, normal), 0.0f);
    float distance = length(lights[i].position - fs_in.fragPos);

    vec3 viewDir = normalize(-fs_in.fragPos);
    vec3 half = normalize(viewDir + lightDir);
    float spec = pow(max(dot(half, normal), 0.0f), material.shininess);

    float attenuation = 1 / (distance * distance);
    if(scene == 0)
        return (diff * lights[i].diffuse * diffuse) * attenuation;
    else
        return (diff * lights[i].diffuse * diffuse + spec * lights[i].specular * specular) * attenuation + lights[i].ambient * diffuse;
}

void main()
{
    vec3 diffuse = texture(material.tex, fs_in.texCoord).xyz;
    vec3 specular = texture(material.specularTex, fs_in.texCoord).xyz;
    vec3 color = vec3(0.0f);
    for(int i = 0; i < LIGHT_NUMBER; i++)
    {
        color += compute_light(i, specular, diffuse);
    }
    fragColor = vec4(color, 1.0f);

    float brightness = dot(color, vec3(0.2126, 0.7152, 0.0722));
    if(brightness > 1.0)
        brightColor = vec4(color, 1.0);
}