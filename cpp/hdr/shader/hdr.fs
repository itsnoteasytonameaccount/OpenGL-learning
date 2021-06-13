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
};


in VS_OUT {
    vec3 fragPos;
    vec3 normal;
    vec2 texCoord;
} fs_in;

#define LIGHT_NUMBER 3;

Light lights[LIGHT_NUMBER];
Material material;

out vec4 fragColor;

vec3 compute_light(int i)
{
    vec3 lightDir = normalize(fs_in.fragPos - lights[i].position);
    float diff = max(dot(-lightDir, fs_in.normal), 0.0f);

    vec3 viewDir = normalize(-fs_in.fragPos);
    vec3 half = normalize(viewDir - lightDir);
    float spec = pow(max(dot(half, fs_in.normal), 0.0f), material.shininess);
    
    return spec * lights[i].specular + diff * lights[i].diffuse + lights[i].ambient;
}

void main()
{
    vec3 texel = texture(material.tex, fs_in.texCoord).xyz;
    vec3 color(0.0f);
    float gamma = 2.2f;
    for(int i = 0; i < LIGHT_NUMBER; i++)
    {
        color += compute_light(i) * texel;
    }
    color = pow(color, vec3(1.0f / gamma));
    fragColor = vec4(color, 1.0f);
}