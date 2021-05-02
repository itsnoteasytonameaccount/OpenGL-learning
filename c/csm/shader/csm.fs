#version 330 core

struct Light
{
    vec3 direction;

    vec3 specular;
    vec3 diffuse;
    vec3 ambient;
};

struct Material {
    sampler2D tex;
    float shininess;
};

uniform Light light;
uniform float z_values[3];
uniform mat4 light_projection_view[3];
uniform sampler3D shadow_texel;

in VS_OUT {
    vec4 FragPosCamera;
    vec4 FragPosWorld;
    vec3 Normal;
    vec3 TexCoord;
} fs_in;

float compute_shadow()
{
    int split = 0;
    for(int i = 0; i < 3; i++)
    {
        if(z_values[i] >= fs_in.FragPosCamera.z)
        {
            split = i;
        }
    }
    vec4 FragPosLight = light_projection_view[i] * fs_in.FragPosWorld;
    vec3 FragPos = vec3(0.5f) * ((FragPosLight.xyz / FragPosLight.w) + vec3(1.0f));
    float depth = FragPos.z;
    float closest_depth = texture(shadow_texel, vec3(FragPos.xy, i)).r;
    return depth < closest_depth ? 1.0f : 0.0f;
}

vec3 compute_light()
{
    vec3 lightDir = light.direction;
    float distance = length(lightDir);
    lightDir = normalize(lightDir);
    float diff = max(dot(-lightDir, fs_in.Normal), 0);
    vec3 diffuse = diff * light.diffuse;

    vec3 viewDir = normalize(view_pos - fs_in.FragPosWorld.xyz);
    vec3 half = normalize(-lightDir + viewDir);
    float spec = pow(max(dot(half, fs_in.Normal), 0.0f), material.shininess);

    vec3 specular = spec * light.specular;
    vec3 ambient = light.ambient;

    float shadow = compute_shadow(lightDir, distance);
    float attenuation = 1 / (distance * distance);
    
    return (specular + diffuse) * shadow + ambient;
}

void main()
{
    vec3 texel = texture(material.tex, fs_in.TexCoord).rgb;
    texel = pow(texel, vec3(2.2));

    vec3 color = vec3(0.0f);
    color = compute_light() * texel;
    color = pow(color, vec3(1/2.2));

    FragColor = vec4(color, 1.0f);
    // FragColor = vec4(1.0f);
}