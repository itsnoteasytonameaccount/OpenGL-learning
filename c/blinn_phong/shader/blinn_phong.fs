#version 330 core

struct Light {
    vec3 position;

    vec3 diffuse;
    vec3 specular;
    vec3 ambient;
};

struct Material {
    sampler2D tex;

    float shininess;
};

uniform sampler2D tex;
uniform Light lights[4];
uniform Material material;
uniform int mode;
uniform int quadratic;
uniform float gamma;
out vec4 FragColor;

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoord;
} fs_in;

float phong_spec(vec3 lightDir)
{
    vec3 viewDir = normalize(-fs_in.FragPos);
    vec3 reflectDir = reflect(lightDir, fs_in.Normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);
    return spec;
}

float blinn_phong_spec(vec3 lightDir)
{
    vec3 viewDir = normalize(-fs_in.FragPos);
    vec3 half = normalize(-lightDir + viewDir);
    float spec = pow(max(dot(half, fs_in.Normal), 0.0f), material.shininess);
    return spec;
}

vec3 compute_light(int i)
{
    Light light = lights[i];
    vec3 lightDir = fs_in.FragPos - light.position;
    float distance = length(lightDir);
    float attenuation;
    lightDir = normalize(lightDir);
    float diff = max(dot(-lightDir, fs_in.Normal), 0);
    vec3 diffuse = diff * light.diffuse;
    float spec;
    if(mode == 0)
    {
        spec = phong_spec(lightDir);
    }
    else
    {
        spec = blinn_phong_spec(lightDir);
    }
    vec3 specular = spec * light.specular;
    vec3 ambient = light.ambient;
    if(quadratic == 1)
    {
        attenuation = 1 / (distance * distance);
    }
    else
    {
        attenuation = 1 / distance;
    }
    return (specular + diffuse) * attenuation + ambient;
}

void main()
{
    vec3 texel = texture(material.tex, fs_in.TexCoord).rgb;
    texel = pow(texel, vec3(gamma));
    vec3 color = vec3(0.0f);
    for(int i = 0; i < 4; i++)
    {
        color += compute_light(i) * texel;
    }
    color = pow(color, vec3(1/gamma));
    FragColor = vec4(color, 1.0f);
}