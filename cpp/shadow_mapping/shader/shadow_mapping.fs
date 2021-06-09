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

uniform sampler2D shadow_texel;
uniform Light light;
uniform Material material;
out vec4 FragColor;

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec4 LightSpaceFragPos;
    vec2 TexCoord;
} fs_in;

float compute_shadow(vec3 lightDir)
{
    vec3 light_space_frag_pos = (fs_in.LightSpaceFragPos.xyz / fs_in.LightSpaceFragPos.w + 1.0f) * 0.5f;
    float shadow = 0.0f;
    float bias = 0.0f;//max(0.05f * (1.0f + dot(lightDir, fs_in.Normal)), 0.005);
    float frag_depth = light_space_frag_pos.z;
    vec2 texelSize = 1.0 / textureSize(shadow_texel, 0);
    if(frag_depth > 1)
    {
        return 1.0f;
    }
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float depth = texture(shadow_texel, light_space_frag_pos.xy + vec2(x, y) * texelSize).r;
            shadow += depth < frag_depth - bias ? 0.0f : 1.0f;
        }
    }
    // float bias = max(0.05f * (1.0f + dot(lightDir, fs_in.Normal)), 0.005);
    return shadow / 9.0f;
}

vec3 compute_light()
{
    vec3 lightDir = fs_in.FragPos - light.position;
    float distance = length(lightDir);
    lightDir = normalize(lightDir);
    float diff = max(dot(-lightDir, fs_in.Normal), 0);
    vec3 diffuse = diff * light.diffuse;

    vec3 viewDir = normalize(-fs_in.FragPos);
    vec3 half = normalize(-lightDir + viewDir);
    float spec = pow(max(dot(half, fs_in.Normal), 0.0f), material.shininess);

    vec3 specular = spec * light.specular;
    vec3 ambient = light.ambient;

    float shadow = compute_shadow(lightDir);
    float attenuation = 1 / (distance * distance);
    
    return (specular + diffuse) * shadow * attenuation + ambient;
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