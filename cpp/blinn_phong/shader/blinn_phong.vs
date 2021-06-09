#version 330 core

layout (location = 0) in vec3 aFragPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

layout (std140) uniform Matrices {
    mat4 projection;
    mat4 view;
};

uniform mat3 normalMatrix;
uniform mat4 model;

out VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoord;
} vs_out;

void main()
{
    vs_out.FragPos = vec3(view * model * vec4(aFragPos, 1.0f));
    vs_out.Normal = normalize(normalMatrix * aNormal);
    vs_out.TexCoord = aTexCoords;
    gl_Position = projection * view * model * vec4(aFragPos, 1.0f);
    // gl_Position = vec4(aFragPos, 1.0f);
}