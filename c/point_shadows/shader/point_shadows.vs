#version 330 core

layout (location = 0) in vec3 aFragPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

layout (std140) uniform Matrices
{
    mat4 projection;
    mat4 view;
};

uniform mat4 model;
uniform int reverse_normal;

out VS_OUT 
{
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoord;
} vs_out;

void main()
{
    mat3 normalMatrix = mat3(transpose(inverse(model)));
    gl_Position = model * vec4(aFragPos, 1.0f);
    vs_out.FragPos = gl_Position.xyz;
    gl_Position = projection * view * gl_Position;
    vs_out.Normal = normalize(normalMatrix * aNormal);
    if(reverse_normal == 1)
    {
        vs_out.Normal = -vs_out.Normal;
    }
    vs_out.TexCoord = aTexCoord;
}