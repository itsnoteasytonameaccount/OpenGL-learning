#version 330 core

layout (location = 0) in vec3 aFragPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

uniform mat4 projection;
uniform mat4 model;
uniform mat4 view;
uniform mat3 normalMatrix;
uniform vec3 texCoordTrans;

out VS_OUT {
    vec3 fragPos;
    vec3 normal;
    vec2 texCoord;
} vs_out;

void main()
{
    vs_out.fragPos = view * model * vec4(aFragPos, 1.0f);
    vs_out.normal = normalize(normalMatrix * aNormal);
    vs_out.texCoord = texCoordTrans.xy * aTexCoord;
    gl_Position = projection * vs_out.fragPos;
}