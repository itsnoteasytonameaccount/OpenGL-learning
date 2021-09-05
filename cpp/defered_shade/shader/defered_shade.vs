#version 330 core

layout (position = 0) in vec3 aFragPos;
layout (position = 1) in vec3 aNormal;
layout (position = 2) in vec2 aTexCoord;

out VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoord;
} vs_out;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform mat3 normalMatrix;

void main()
{
    vec4 fragPos = view * model * vec4(aFragPos, 1.0f);
    vs_out.TexCoord = aTexCoord;
    vs_out.Normal = normalize(normalMatrix * aNormal);
    gl_Position = projection * fragPos;
    vs_out.FragPos = fragPos.xyz;
}