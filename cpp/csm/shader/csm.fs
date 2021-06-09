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

#define SPLIT_QUANTITY 3

uniform Light light;
uniform float z_values[SPLIT_QUANTITY];
uniform mat4 light_projection_view[SPLIT_QUANTITY];
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

vec2 offsets[9] = vec2[](
    vec2(1.0f, 1.0f),
    vec2(1.0f, -1.0f),
    vec2(1.0f, 0.0f),
    vec2(0.0f, 1.0f),
    vec2(0.0f, -1.0f),
    vec2(0.0f, 0.0f),
    vec2(-1.0f, 1.0f),
    vec2(-1.0f, -1.0f),
    vec2(-1.0f, 0.0f)
);

float compute_shadow()
{
    int samplers = 9;
    for(int i = 0; i < SPLIT_QUANTITY; i++)
    {
        if(z_values[i] >= -fs_in.FragPosCamera.z)
        {
            split = i;
            break;
        }
    }
    float bias = 0.0001;
    vec4 FragPosLight = light_projection_view[split] * fs_in.FragPosWorld;
    vec3 FragPos = 0.5f * ((FragPosLight.xyz / FragPosLight.w) + 1.0f);
    float depth = FragPos.z;
    ivec3 tsize = textureSize(shadow_texel, 0);
    float shadow = 0.0f;
    for(int j = 0; j < 9; j++)
    {
        vec2 offset = vec2(1.0f / tsize.x, 1.0f / tsize.y) * offsets[j];
        float closest_depth = texture(shadow_texel, vec3(FragPos.xy + offset, float(split))).r;
        shadow += depth <= closest_depth + bias ? 1.0f : 0.0f;
    }
    return shadow / 9.0f;
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
}