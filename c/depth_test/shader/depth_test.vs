#version 330 core

layout (location = 0) in vec3 aFragPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

uniform mat4 view;
uniform mat4 projection;
uniform mat4 model;

out vec2 TexCoords;

void main()
{
    gl_Position = projection * view * model * vec4(aFragPos, 1.0f);
    TexCoords = aTexCoords;
}