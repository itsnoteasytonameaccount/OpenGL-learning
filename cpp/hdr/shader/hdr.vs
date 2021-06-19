#version 330 core

layout (location = 0) in vec3 aFragPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

uniform mat4 projection;
uniform mat4 model;
uniform mat4 view;
uniform mat3 normalMatrix;
uniform bool normalFlip;

out VS_OUT {
    vec3 fragPos;
    vec3 normal;
    vec2 texCoord;
} vs_out;

void main()
{
    vec4 fragPos = view * model * vec4(aFragPos, 1.0f);
    vs_out.normal = normalize(normalMatrix * (normalFlip? -aNormal : aNormal));
    vs_out.texCoord = aTexCoord;
    vs_out.fragPos = fragPos.xyz;
    gl_Position = projection * fragPos;
}