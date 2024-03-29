#version 330 core

layout (location = 0) in vec3 aFragPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTextCoord;
layout (location = 3) in vec3 tangent;
layout (location = 4) in vec3 bitangent;

struct Light
{
    vec3 position;

    vec3 diffuse;
    vec3 ambient;
    vec3 specular;
};

uniform mat4 projection;
uniform mat4 model;
uniform mat4 view;
uniform mat3 normal_matrix;
uniform Light light;

out VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoord;
    mat3 TBN;
    vec3 TangentLightPos;
    vec3 TangentFragPos;
} vs_out;

void main()
{
    vec4 FragPos = view * model * vec4(aFragPos, 1.0f);
    vs_out.FragPos = vec3(FragPos);
    gl_Position = projection * FragPos;
    vs_out.Normal = normalize(normal_matrix * aNormal);
    vs_out.TexCoord = aTextCoord;

   vec3 T = normalize(normal_matrix * tangent);
   vec3 B = normalize(normal_matrix * bitangent);
   vec3 N = normalize(normal_matrix * aNormal);
   vs_out.TBN = mat3(T, B, N);
   
   vs_out.TangentLightPos = transpose(vs_out.TBN) * light.position;
   vs_out.TangentFragPos  = transpose(vs_out.TBN) * vs_out.FragPos;
}