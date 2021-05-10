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
uniform sampler2DArray shadow_texel;
uniform vec3 view_pos;
uniform Material material;
uniform int floor;

in VS_OUT {
    vec4 FragPosCamera;
    vec4 FragPosWorld;
    vec3 Normal;
    vec2 TexCoord;
} fs_in;

out vec4 FragColor;

int split = 0;

float compute_shadow()
{
    for(int i = 0; i < 3; i++)
    {
        if(z_values[i] >= -fs_in.FragPosCamera.z)
        {
            split = i;
            break;
        }
    }
    float bias = 0.0001;
    // if(split == 0)
    // {
    //     FragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);
    // }
    // else if(split == 1)
    // {
    //     FragColor = vec4(0.0f, 1.0f, 0.0f, 1.0f);
    // }
    // else
    // {
    //     FragColor = vec4(0.0f, 0.0f, 1.0f, 1.0f);
    // }
    vec4 FragPosLight = light_projection_view[split] * fs_in.FragPosWorld;
    vec3 FragPos = 0.5f * ((FragPosLight.xyz / FragPosLight.w) + 1.0f);
    float depth = FragPos.z;
    float closest_depth = texture(shadow_texel, vec3(FragPos.xy, float(split))).r;
    return depth <= closest_depth + bias ? 1.0f : 0.0f;
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

    float shadow = compute_shadow();
    float attenuation = 1 / (distance * distance);
    
    return (specular + diffuse) * shadow + ambient;
    // return vec3(1.0f);
}

void main()
{
    vec3 texel = texture(material.tex, fs_in.TexCoord).rgb;
    texel = pow(texel, vec3(2.2));

    vec3 color = compute_light();
    if(floor == 1)
    {
        if(split == 0)
        {
            color *= vec3(1.0f, 0.0f, 0.0f);
        }
        else if(split == 1)
        {
            color *= vec3(0.0f, 1.0f, 0.0f);
        }
        else
        {
            color *= vec3(0.0f, 0.0f, 1.0f);
        }
    }
    else
    {
        color *= texel;
    }
    color = pow(color, vec3(1/2.2));
    
    FragColor = vec4(color, 1.0f);
    // FragColor = vec4(texel, 1.0f);
    // FragColor = vec4(0.0f);
}