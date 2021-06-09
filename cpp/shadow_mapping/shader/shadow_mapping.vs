#version 330 core

layout (location = 0) in vec3 aFragPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

layout (std140) uniform Matrices {
    mat4 projection;
    mat4 view;
    mat4 lightSpaceMatrix;
};

uniform mat3 normalMatrix;
uniform mat4 model;

out VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec4 LightSpaceFragPos;
    vec2 TexCoord;
} vs_out;

void main()
{
    vec4 temp = model * vec4(aFragPos, 1.0f);
    vs_out.LightSpaceFragPos = lightSpaceMatrix * temp;
    vs_out.Normal = normalize(normalMatrix * aNormal);
    temp = view * temp;
    vs_out.FragPos = vec3(temp);
    vs_out.TexCoord = aTexCoord;
    gl_Position = projection * temp;
    // gl_Position =  vec4(aFragPos, 1.0f);
}