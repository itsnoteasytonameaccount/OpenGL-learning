#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

uniform mat4 view;
uniform mat4 model;
uniform mat4 projection;
uniform mat3 normalMatrix;

out vec4 FragPos;
// out vec2 TexCoords;
// out vec4 Normal;

out VS_OUT {
    vec2 texCoords;
    vec4 normal;
} vs_out;

void main()
{
    FragPos = view * model * vec4(aPos, 1.0f);
    gl_Position = projection * FragPos;

    // Normal = vec4(normalize(normalMatrix * aNormal), 0.0f);
    vs_out.normal = vec4(normalize(normalMatrix * aNormal), 0.0f);
    // TexCoords = aTexCoords;
    vs_out.texCoords = aTexCoords;
}