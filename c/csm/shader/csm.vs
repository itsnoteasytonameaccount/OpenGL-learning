#version 330 core

layout (location = 0) in vec3 aFragPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in mat4 model;

uniform mat4 model2;
uniform mat4 view;
uniform mat4 projection;
uniform int use_uniform;

out VS_OUT {
    vec4 FragPosCamera;
    vec4 FragPosWorld;
    vec3 Normal;
    vec2 TexCoord;
} vs_out;

void main()
{
    mat4 used_model = use_uniform == 1 ? model2 : model;
    vec4 FragPos = used_model * vec4(aFragPos, 1.0f);
    vs_out.FragPosWorld = FragPos;
    FragPos = view * FragPos;
    vs_out.FragPosCamera = FragPos;
    vs_out.Normal = normalize(vec3(inverse(transpose(used_model)) * vec4(aNormal, 1.0f)));
    vs_out.TexCoord = aTexCoord;
    gl_Position = projection * FragPos;
}