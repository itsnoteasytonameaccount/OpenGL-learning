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

uniform samplerCube shadow_texel;
uniform Light light;
uniform Material material;
uniform float far_plane;
uniform vec3 view_pos;

out vec4 FragColor;
vec3 sampleOffsetDirections[20] = vec3[]
(
   vec3( 1,  1,  1), vec3( 1, -1,  1), vec3(-1, -1,  1), vec3(-1,  1,  1), 
   vec3( 1,  1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1,  1, -1),
   vec3( 1,  1,  0), vec3( 1, -1,  0), vec3(-1, -1,  0), vec3(-1,  1,  0),
   vec3( 1,  0,  1), vec3(-1,  0,  1), vec3( 1,  0, -1), vec3(-1,  0, -1),
   vec3( 0,  1,  1), vec3( 0, -1,  1), vec3( 0, -1, -1), vec3( 0,  1, -1)
);

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoord;
} fs_in;

float compute_shadow(vec3 lightDir, float distance)
{
    int samples = 20;
    float shadow = 0.0f;
    float bias = 0.005;
    distance = distance / far_plane;
    float diskRadius = distance / 25.0f;
    for(int i = 0; i < samples; i++)
    {
        float depth = texture(shadow_texel, lightDir + sampleOffsetDirections[i] * diskRadius).r;
        shadow += distance - bias < depth ? 1.0f : 0.0f;
    }
    shadow /= float(samples);
    return shadow;
}

vec3 compute_light()
{
    vec3 lightDir = fs_in.FragPos - light.position;
    float distance = length(lightDir);
    lightDir = normalize(lightDir);
    float diff = max(dot(-lightDir, fs_in.Normal), 0);
    vec3 diffuse = diff * light.diffuse;

    vec3 viewDir = normalize(view_pos - fs_in.FragPos);
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