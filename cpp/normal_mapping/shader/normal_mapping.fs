#version 330 core

struct Light
{
    vec3 position;

    vec3 diffuse;
    vec3 ambient;
    vec3 specular;
};

struct Material
{
    sampler2D texture_specular1;
    sampler2D texture_diffuse1;
    sampler2D texture_normal;
    sampler2D texture_disp;

    float shininess;
};

in VS_OUT
{
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoord;
    mat3 TBN;
    vec3 TangentLightPos;
    vec3 TangentFragPos;
} fs_in;

uniform Material material;
uniform Light light;
uniform int use_normal_map;
uniform int use_disp_map;
uniform float height_scale;
uniform int divide;

out vec4 FragColor;

vec2 comput_texcoord(vec2 TexCoord)
{
    vec3 view_dir = normalize(-fs_in.TangentFragPos);
    if(view_dir.z == 0.0f)
    {
        return TexCoord;
    }
    float depth_change_rate = 1.0f / divide;
    vec2 direction = view_dir.xy * depth_change_rate / view_dir.z * height_scale;
    float current_layer_depth = 0.0f;
    float current_map_depth = texture(material.texture_disp, TexCoord).r;
    float prev_map_depth = current_map_depth;
    vec2 current_texcoord = TexCoord;
    while(current_layer_depth < current_map_depth)
    {
        prev_map_depth = current_map_depth;
        current_texcoord -= direction;
        current_map_depth = texture(material.texture_disp, current_texcoord).r;
        current_layer_depth += depth_change_rate;
    }

    float after_depth = current_map_depth - current_layer_depth;
    float before_depth = prev_map_depth - current_layer_depth + depth_change_rate;

    float weight = after_depth / (after_depth - before_depth);
    current_texcoord = current_texcoord + weight * direction; 
    return current_texcoord;
}

vec3 compute_light(vec3 normal, vec3 texture_diffuse, vec3 texture_specular)
{
    vec3 light_dir = light.position - fs_in.FragPos;
    float distance = length(light_dir);

    light_dir = normalize(light_dir);
    float diff = max(dot(light_dir, normal), 0.0f);

    vec3 view_dir = normalize(-fs_in.FragPos);
    vec3 half_dir = normalize(view_dir + light_dir);
    float spec = pow(max(dot(half_dir, normal), 0.0f), material.shininess);

    float attenuation = 1 / (distance * distance);

    return spec * light.specular * texture_specular + diff * light.diffuse * texture_diffuse + light.ambient * texture_diffuse;
}

void main()
{
    vec2 TexCoord = use_disp_map == 1 ? comput_texcoord(fs_in.TexCoord) : fs_in.TexCoord;
    vec3 diffuse = texture(material.texture_diffuse1, TexCoord).xyz;
    vec3 normal = texture(material.texture_normal, TexCoord).xyz;
    vec3 specular = texture(material.texture_specular1, TexCoord).xyz;
    // texel = pow(texel, vec3(2.2));
    normal = normalize(normal * 2.0f - 1.0f);
    normal = normalize(fs_in.TBN * normal);
    if(use_normal_map == 1)
    {
        normal = normalize(fs_in.Normal);
    }

    vec3 color = compute_light(normal, diffuse, specular);
    // mat3 matrix = mat3(vec3(0.0, 1.0, 0.0), vec3(0.0), vec3(0.0));
    // vec3 result = matrix * vec3(1.0);
    // color = pow(color, vec3(1 / 2.2));
    FragColor = vec4(color, 1.0f);
    // FragColor = vec4(result, 1.0f);
}