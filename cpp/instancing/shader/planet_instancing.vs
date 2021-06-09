#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in mat4 model2;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat3 normalMatrix;
uniform int type;

out VS_OUT {
    vec4 position;
    vec4 normal;
    vec2 texCoords;
} vs_out;

void main()
{
    if(type == 1)
    {
        vs_out.position = view * model * vec4(aPos, 1.0f);
    }
    else
    {
        vs_out.position = view * model2 * vec4(aPos, 1.0f);
    }
    gl_Position = projection * vs_out.position;
    vs_out.normal = vec4(normalize(normalMatrix * aNormal), 0.0f);
    vs_out.texCoords = aTexCoords;
}